#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "main.h"
#include "oled.h"
#include <cstring>
#include "pattern/picture.h"
#include "blackboard.h"
#include "../../data_def.h"
#include <stdio.h>


extern I2C_HandleTypeDef hi2c1;
extern ADC_HandleTypeDef hadc1;



extern __attribute__((section(".sram2"))) uint8_t zero[128];
extern __attribute__((section(".sram2"))) uint8_t full[128];
extern __attribute__((section(".sram2"))) uint8_t oled_buffer[8][128];
__attribute__((section(".sram2"))) uint16_t adc_buffer[2]; 
int input[2];
char message1[10];
char message2[10];


const char* song_name_list[]=
{
    "Before the Story",
    "BIG SHOT",
    "Black Knife",
    "DELTARUNE ED",
    "Festival",
    "FieldOfHopes&Dreams",
    "Flower Man",
    "Hammer of Justice",
    "Cutie MewMew Magic",
    "Petal Dance",
    "Raise up Your Bat",
    "Running Sky",
    "Shop3",
    "Sunset of SevenSuns",
    "The Third Sanctuary",
    "THE WORLD REVOLVING",
    "TV WORLD",
    "who might you be",
    "Paradise, Paradise",
    "Ruder Buster",
    "FlowerCastle",
    "VioletTactics",
    "WalkingHome",

    "Undyne",
    "ASGORE",

};


extern "C"
{
    extern void oled_task(void *argument)
    {
        pen::instance().
            set_position(0,0).
            set_mode(0).
            set_line_height(8);

        
        HAL_ADCEx_Calibration_Start(
            &hadc1, ADC_CALIB_OFFSET_LINEARITY, ADC_SINGLE_ENDED);
        HAL_ADCEx_Calibration_Start(
            &hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);
        HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 2);


        
        while(true)
        {

            static int tick = 0;
            memset(oled_arr, 0, 1024);



            menuctx cmd;
            blackboard::instance().read_menu_ctx(&cmd);
            if(cmd.current_state == MenuState::MAIN)
            {
                pen::instance().
                set_position(0, 0).
                set_line_height(8).
                draw_string("MAIN").
                set_position(10, 20).
                draw_string("MUSIC").
                set_position(10, 30).
                draw_string("STANDBY").
                set_position(10, 40).
                draw_string("DEBUG").
                
                set_position(0, 20+10*cmd.current_index).
                draw_char('*').
                set_position(80, 15).
                draw_pattern(picture_Ralsei_face_battlemenuData);
            }

            else if(cmd.current_state == MenuState::MUSIC)
            {
                pen::instance().
                set_position(0, 0).
                set_line_height(8).
                draw_string("MUSIC");
                if(cmd.current_index >= 2)
                {
                    pen::instance().
                    set_position(10, 15).
                    draw_string(song_name_list[cmd.current_index-2]);
                }
                if(cmd.current_index >= 1)
                {
                    pen::instance().
                    set_position(10, 25).
                    draw_string(song_name_list[cmd.current_index-1]);
                }

                pen::instance().
                set_position(10, 35).
                draw_string(song_name_list[cmd.current_index]).
                set_position(0, 35).
                draw_char('*');

                if(cmd.current_index <=MUSIC_MENU_MAX_NUM-2)
                {
                    pen::instance().
                    set_position(10, 45).
                    draw_string(song_name_list[cmd.current_index+1]);
                }
                if(cmd.current_index <=MUSIC_MENU_MAX_NUM-3)
                {
                    pen::instance().
                    set_position(10, 55).
                    draw_string(song_name_list[cmd.current_index+2]);
                }
            }



            else if(cmd.current_state == MenuState::STANDBY)
            {
                pen::instance().
                set_position(0, 0).
                draw_gif(gif_Pink_face_shop_animation_Orange, tick, 30);
            }


            else if(cmd.current_state == MenuState::PLAYING_MUSIC)
            {
                pen::instance().
                set_position(0, 0).
                draw_string(song_name_list[cmd.current_music_index]).
                set_position(30, 28);
                if(cmd.current_playing_state == MusicPlayingState::PLAYING)
                {
                    pen::instance().
                    draw_string("Playing...");
                }
                else if(cmd.current_playing_state == MusicPlayingState::STOP)
                {
                    pen::instance().
                    draw_string("S.T.O.P");
                }

                pen::instance().
                set_position(30, 40).
                draw_string("loop : ");
                if(cmd.music_is_looped)
                {
                    pen::instance().draw_string("TRUE");
                }
                else 
                {
                    pen::instance().draw_string("FALSE");
                }   
            }
            else if(cmd.current_state == MenuState::_DEBUG)
            {
                
                
                input[0] = (adc_buffer[0]-2047) / 2048.0f *100;
                input[1] = (adc_buffer[1]-2047) / 2048.0f *100;
                sprintf(message1, "%d", input[0]);
                sprintf(message2, "%d", input[1]);
                pen::instance().
                set_position(30, 20).
                draw_string(message1).
                set_position(30, 30).
                draw_string(message2);
            }
            
            

            memcpy(oled_buffer, oled_arr, 1024);
            tick++;
            osSemaphoreRelease(send_sem);
            vTaskDelay(pdMS_TO_TICKS(30));
        }
    }
    void oled_i2c_task(void *argument)
    {
        osSemaphoreAttr_t attr = {0};
        attr.name = "oled_send_sem";
        send_sem = osSemaphoreNew(1, 1, &attr);

        for(int j=0;j<128;j++)
        {
            zero[j] = 0;
            full[j] = 0xFF;
        }
        vTaskDelay(50);
        oled_init();
        while(true)
        {
            osSemaphoreAcquire(send_sem, osWaitForever);
            write_full_screen(oled_buffer);
        }
    }
}






