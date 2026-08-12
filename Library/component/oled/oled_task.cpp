#include "FreeRTOS.h"
#include "projdefs.h"
#include "task.h"
#include "main.h"
#include "oled.h"
#include <cstring>
#include "pattern/picture.h"
#include "../../Blackboard/blackboard.h"
#include "../../data_def.h"

extern I2C_HandleTypeDef hi2c1;



extern __attribute__((section(".sram2"))) uint8_t zero[128];
extern __attribute__((section(".sram2"))) uint8_t full[128];
extern __attribute__((section(".sram2"))) uint8_t oled_buffer[8][128];

extern "C"
{
    extern void oled_task(void *argument)
    {
        pen::instance().
            set_position(0,0).
            set_mode(0).
            set_line_height(8);
        while(true)
        {

            static int tick = 0;
            memset(oled_arr, 0, 1024);
            
            uart_cmd cmd;
            blackboard::instance().read_uart_cmd(&cmd);

            char data[8];
            
            if(cmd.msg.w)
            {
                data[0] = 'T';
            }
            else 
            {
                data[0] = ' ';
            }
            if(cmd.msg.a)
            {
                data[1] = 'T';
            }
            else 
            {
                data[1] = ' ';
            }
            if(cmd.msg.s)
            {
                data[2] = 'T';
            }
            else 
            {
                data[2] = ' ';
            }
            if(cmd.msg.d)
            {
                data[3] = 'T';
            }
            else 
            {
                data[3] = ' ';
            }
            if(cmd.msg.z)
            {
                data[4] = 'T';
            }
            else 
            {
                data[4] = ' ';
            }
            if(cmd.msg.x)
            {
                data[5] = 'T';
            }
            else 
            {
                data[5] = ' ';
            }
            if(cmd.msg.c)
            {
                data[6] = 'T';
            }
            else 
            {
                data[6] = ' ';
            }
            


            pen::instance().
                set_position(0, 0).
                set_line_height(8).
                draw_string(data);



            // //做一个骑士图案上下浮动的gif，浮动高度5像素

            // static int a=0;
            // static int step = 1;
            // if(tick%5==0)
            // {
            //     if(a==0)
            //     {
            //         step = 1;
            //     }
            //     if(a==4)
            //     {
            //         step = -1;
            //     }
            //     a+=step;
            // }


            // pen::instance().
            //     set_position(30, 0+a).
            //     set_line_height(8).
            //     draw_pattern(picture_Knight_battle_idleData);



            // pen::instance().
            //     set_position(0, 0).
            //     set_line_height(8).
            //     draw_string("KnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnightKnight", 0, 1);
                

            
            

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






