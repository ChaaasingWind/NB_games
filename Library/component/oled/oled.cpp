#include "oled.h"
#include "stdint.h"
#include "main.h"
#include "stm32h7xx_hal_i2c.h"
#include "FreeRTOS.h"
#include "task.h" 
#include "cmsis_os.h" 


__attribute__((section(".sram2"))) uint8_t zero[128] = {0};
__attribute__((section(".sram2"))) uint8_t full[128] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
__attribute__((section(".sram2"))) uint8_t oled_buffer[8][128];
static osSemaphoreId_t dma_sem;

void oled_dma_init(void)
{
    osSemaphoreAttr_t attr = {0};
    attr.name = "oled_dma_sem";
    dma_sem = osSemaphoreNew(1, 1, &attr);
}

void oled_write_cmd(uint8_t cmd)
{
    HAL_I2C_Mem_Write(&hi2c1, 0x3C<<1, 0x00, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
}



void oled_write_data(uint8_t* txptr, uint16_t size)
{
    dma_done = false;
    HAL_I2C_Mem_Write_DMA(&hi2c1, 0x3C<<1, 0x40, I2C_MEMADD_SIZE_8BIT, txptr, size);
    
    osStatus_t sem_status = osSemaphoreAcquire(dma_sem, pdMS_TO_TICKS(100));
    // 检查等待结果
    if (sem_status != osOK) 
    {
        return;
    }
}

void oled_init()
{
    oled_dma_init();
    oled_write_cmd(0xAE);
    oled_write_cmd(0xD5);
    oled_write_cmd(0x80);
    oled_write_cmd(0xA8);
    oled_write_cmd(0x3F);
    oled_write_cmd(0xD3);
    oled_write_cmd(0x00);
    oled_write_cmd(0x40);
    oled_write_cmd(0x8D);
    oled_write_cmd(0x14);
    clear_screen();
    oled_write_cmd(0xA4);
    oled_write_cmd(0xAF);
}

void oled_update_page(uint8_t page, const uint8_t* data)
{
    if(page<=7)
    {
        oled_write_cmd(0xB0+page);
        oled_write_cmd(0x00);
        oled_write_cmd(0x10);
        oled_write_data((uint8_t*)data, 128);
    }
}

void clear_screen()
{
    for(int i = 0 ; i < 8 ; i++)
    {

        oled_update_page( i , zero);
    }
}
void write_full_screen(uint8_t buffer[8][128])
{
    for(int i = 0 ; i < 8 ; i++)
    {
        oled_update_page( i , buffer[i]);
        //oled_update_page( i , full);
    }
}





extern "C" void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if(hi2c->Instance == I2C1)
    {
        dma_done = true;
        osSemaphoreRelease(dma_sem);
    }
}

extern "C" void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
        osSemaphoreRelease(dma_sem);  // ★ 释放信号量，让等待的任务退出
    }
}



//上层画布画法

//最基础画法：画一个点
void basic_draw_bit(uint8_t x, uint8_t y, uint8_t mode)
{
    y = 63 - y;
    x = 127 - x;
    if(x >= 128 || y >= 64)
    {
        return;
    }
    if(mode == 0)
    {
        oled_arr[y / 8][x] |= 1<<(y % 8);
    }
    else if(mode == 1)
    {
        oled_arr[y / 8][x] &= ~(1<<(y % 8));
    }
    else if(mode == 2)
    {
        oled_arr[y / 8][x] ^= (1<<(y % 8));
    }
    
}



pen& pen::set_position(uint8_t x, uint8_t y)
{
    this->x = x;
    this->y = y;
    return *this;
}


pen& pen::set_mode(uint8_t mode)
{
    //mode
    // 0: 写为1
    // 1：写为0
    // 2：反转
    this->mode = mode;
    return *this;
}

pen& pen::set_line_height(uint8_t height)
{
    this->line_height = height;
    return *this;
}

bool pen::check_bit(uint8_t x, uint8_t y)
{
    y = 63 - y;
    x = 127 - x;
    if(x >= 128 || y >= 64)
    {
        return false;
    }
    return (oled_arr[y / 8][x] & (1<<(y % 8))) != 0;
}

pen& pen::draw_char(const char c, uint8_t height, uint8_t width)
{
    //字符合法性判断
    if(c < ' ' || c > '~')
    {
        return *this;
    }
    if(height == 8 && width == 6)
    {
        for(uint8_t i = 0; i<height; i++)
        {
            for(uint8_t j = 0; j<width; j++)
            {
                if(Font6x8[(c - ' ')*height+i] & (1<<(16-j)))
                {
                    basic_draw_bit(x+j,y+i,mode);
                }
            }
        }
    }
    
    return *this;
}
pen& pen::draw_string(const char* str, uint8_t interval, uint8_t write_mode)
{
    if(write_mode == 0)
    {
        //line_height是每行的高度，默认是8，如果有更高的字符，可以设置line_height为更高的值
        if(line_height < 8)
        {
            line_height = 8;
        }
        while(*str != '\0')
        {
            if(x+6 >= 128)
            {
                x = 0;
                y += line_height;
                //重置line_height为默认值8
                line_height = 8;
            }
            draw_char(*str);
            x += 6 + interval;

            str++;
        }
    }
    else if(write_mode == 1)
    {
        //这个模式下会让字符自己搜索接下来的空间是否为空，如果有图画挡住就检查下一个字符的位置，直到这个字符可以画下去为止
        //它检查的是这6*8的一整块里面是否有像素
        line_height = 8;
        while(*str != '\0')
        {
            //检查当前字符是否可以画下去
            bool can_draw = false;
            while(!can_draw)
            {
                //换行检测
                if(x+6 >= 128)
                {
                    x = 0;
                    y += line_height;
                    //重置line_height为默认值8
                    line_height = 8;
                }

                can_draw = true;
                for(uint8_t i = 0; i<line_height; i++)
                {
                    for(uint8_t j = 0; j<6; j++)
                    {
                        if(check_bit(x+j, y+i))
                        {
                            can_draw = false;
                            x += 6 + interval;
                            break;
                        }
                    }
                    if(!can_draw)
                    {
                        break;
                    }
                }

            }
            draw_char(*str);
            x += 6 + interval;
            str++;
        }
    }
    
    return *this;
}
pen& pen::enter_new_line(int x)
{
    y += line_height;
    //重置line_height为默认值8
    line_height = 8;
    this->x = x;
    return *this;
}

























pen& pen::draw_bit()
{
    basic_draw_bit(x, y, mode);
    return *this;
}


//画一个长方形
pen& pen::draw_rectangle(uint8_t width, uint8_t height, uint8_t rectangle_mode)
{
    //rectangle_mode
    //0:目标点为长方形的左上角
    //1：目标点为长方形的中心
    if(rectangle_mode == 0)
    {
        uint8_t a =x;
        uint8_t b =y;
        uint8_t c =x+width;
        uint8_t d =y+height;

        for(uint8_t i = a; i < c; i++)
        {
            basic_draw_bit(i, b, mode);
            basic_draw_bit(i, d-1, mode);
        }
        for(uint8_t i = b+1; i < d-1; i++)
        {
            basic_draw_bit(a, i, mode);
            basic_draw_bit(c-1, i, mode);
        }
        
    }
    else if(rectangle_mode == 1)
    {
        uint8_t a =x-width/2;
        uint8_t b =y-height/2;
        uint8_t c =x+width/2;
        uint8_t d =y+height/2;

        for(uint8_t i = a; i < c; i++)
        {
            basic_draw_bit(i, b, mode);
            basic_draw_bit(i, d-1, mode);
        }
        for(uint8_t i = b+1; i < d-1; i++)
        {
            basic_draw_bit(a, i, mode);
            basic_draw_bit(c-1, i, mode);
        }
    }
    return *this;
}

pen& pen::draw_pattern(const image& image)
{
    //设置line_height为图片高度，方便换行
    if(line_height < image.height)
    {
        line_height = image.height;
    }
    for(uint8_t i = 0; i<image.height; i++)
    {
        for(uint8_t j = 0; j<image.width; j++)
        {
            if(*(image.ptr+(i/8)*image.width+j) & (1<<(i%8)))
            {
                basic_draw_bit(x+j,y+i,mode);
            }
        }
    }
    //更新x
    x += image.width;
    return *this;
}



pen& pen::draw_gif(const gif& gif, int tick, uint8_t vel)
{
    static int a=0;
    static int step = 1;
    //设置line_height为gif高度，方便换行
    if(line_height < gif.height)
    {
        line_height = gif.height;
    }
    if(tick%vel==0)
    {
        if(a==0)
        {
            step = 1;
        }
        if(a==gif.frame_number - 1)
        {
            step = -1;
        }
        a+=step;
    }
    for(uint8_t i = 0; i<gif.height; i++)
    {
        for(uint8_t j = 0; j<gif.width; j++)
        {
            if(*((gif.ptr + a * gif.size)+(i/8)*gif.width+j) & (1<<(i%8)))
            {
                basic_draw_bit(x+j,y+i,mode);
            }
        }
    }
    //更新x
    x += gif.width;
    return *this;
}