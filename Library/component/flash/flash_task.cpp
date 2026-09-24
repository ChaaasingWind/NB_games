#include "FreeRTOS.h"
#include "task.h"
#include "w25q64.h"
#include <string.h>




#define FLASH_WRITE_MODE 1
#define CONTENTS_SECTOR_NUM 5

extern OSPI_HandleTypeDef hospi1;







#if FLASH_WRITE_MODE

#include "song_lists.h"
song_flash_data song_list_data[sizeof(song_list)/sizeof(song*)];

extern "C"
{


    void flash_task(void *argument)
    {
        w25q64& flash = w25q64::instance(&hospi1);
        flash.w25q64_init();
        flash.w25q64_disable_memory_mapped();
        if(flash.w25q64_read_id() == 0xEF4017)
        {

            // 1.擦除目录的扇区
            for(int i = 0; i < CONTENTS_SECTOR_NUM; i++)
            {
                flash.w25q64_erase_sector(i * 0x001000);
            }

            // 3.计算所有的声部加起来一共有多少字节，求出所需要擦除的最小扇区数
            int whole_song_size = 0;
            int min_sector_num = 0;
            for(uint32_t i = 0; i < sizeof(song_list)/sizeof(song*); i++)
            {
                for(int j = 0; j < BUZZER_CHANNEL_NUM; j++)
                {
                    whole_song_size += (*(song_list + i))->voice_size[j] * sizeof(sound);
                }
            }
            min_sector_num = (whole_song_size + 0xfff - 1) / 0x1000;

            // 4.擦除对应扇区
            for(int i =0; i < min_sector_num; i++)
            {
                flash.w25q64_erase_sector(CONTENTS_SECTOR_NUM * 0x001000 + i*0x001000);
            }

            // 5.向对应扇区写入，要自动换页
            uint32_t addr = CONTENTS_SECTOR_NUM * 0x001000;
            for(uint32_t i = 0; i < sizeof(song_list)/sizeof(song*); i++)
            {
                strncpy(song_list_data[i].name,
                (*song_list[i]).song_name,sizeof(song_list_data[i].name) - 1);
                song_list_data[i].name[sizeof(song_list_data[i].name) - 1] = '\0';
                
                for(int j = 0; j < BUZZER_CHANNEL_NUM; j++)
                {
                    song_list_data[i].voice_offset[j] = addr;
                    song_list_data[i].voice_num[j] = (*(song_list + i))->voice_size[j];
                    addr = flash.w25q64_write_data(addr, (const uint8_t*)((*(song_list + i))->song_voice[j]), song_list_data[i].voice_num[j] * sizeof(sound));
                }
            }
            // 6.向首页写入目录信息
            flash.w25q64_write_data(0x000000, (const uint8_t*)(song_list_data), sizeof(song_list_data));
            
            while(true)
            {
                HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_7);
                vTaskDelay(pdMS_TO_TICKS(300));
            }
        }
        else 
        {
        
        }



    }
}



#else

#include "flash_song_lists.h"


static song_flash_data* ptr = (song_flash_data*)FLASH_MUSIC_BASE;


extern "C"
{
    void flash_task(void *argument)
    {
        w25q64& flash = w25q64::instance(&hospi1);
        flash.w25q64_init();
        if(flash.w25q64_read_id() == 0xEF4017)
        {

            //开启内存映射模式
            flash.w25q64_enable_memory_mapped();

            //初始化每一首歌曲
            for(int i = 0; i < MUSIC_MENU_MAX_NUM; i++)
            {
                for(int j = 0;j < BUZZER_CHANNEL_NUM; j++)
                {
                    song_list[i].song_voice[j] = (sound*)(FLASH_MUSIC_BASE + (ptr + i)->voice_offset[j]);
                    song_list[i].voice_size[j] = (ptr + i)->voice_num[j];
                }
                song_list[i].song_name = (ptr + i)->name;
                song_list[i].overall_time = song_list[i].get_overall_time();
            }
        }
        else
        {

        }

        vTaskDelete(NULL);
    }
}





#endif











