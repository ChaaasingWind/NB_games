#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "led_animation.h"
#include "tim.h"
#include "dma.h"
#include <charconv>
#include <cstdint>
#include <cstring>
#include <cmath>



extern "C" {

#define CODE_0 110
#define CODE_1 233
#define REAL_ZERO 0

typedef struct
{
    uint16_t hue;
    bool ifhave;
    bool ifpressed = false;
    uint16_t unpressed_time;

}block;

extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;
//extern Button buttonarr[4][4];

RGB RGB_Data[8][8]= {0};
__attribute__((section(".sram2"))) uint16_t PWM_Buffer[64*24+64]  = {0} ;
SemaphoreHandle_t dmaSemaphore;

HSV hsv;
block blockarr[7][4];


void music_game(int circle);



void led_task(void *argument)
{
    
    dmaSemaphore = xSemaphoreCreateBinary();
    srand(HAL_GetTick());
    while(1)
    {
        
        
        memset(PWM_Buffer, 0, sizeof(PWM_Buffer));
        static int circle = 0;
        
        circle++;
        
        led_animation3(RGB_Data, circle);
        //music_game(circle);
        

        
        
        fill(RGB_Data, PWM_Buffer);
        
        vTaskDelay(pdMS_TO_TICKS(1));
        Send(PWM_Buffer, dmaSemaphore);
        vTaskDelay(pdMS_TO_TICKS(1));
        
        
    }

}








void HAL_DMA_XferCpltCallback(DMA_HandleTypeDef *hdma) {
    if (hdma->Instance == hdma_tim3_ch1.Instance) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        xSemaphoreGiveFromISR(dmaSemaphore, &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        
    }
}

    


// void music_game(int circle)
// {
//     int tick = circle;
//     uint8_t num = 0;

//     //如果到达目标更新时间，随机生成方块，再让所有方块掉下一格
//     if(tick % 100 == 0)
//     {
//         //先让所有方块掉下一格，最后一行直接被上一行覆盖
//         for(int i = 5; i >= 0; i--)
//         {
//             for(int j = 0; j < 4; j++)
//             {
//                 blockarr[i+1][j] = blockarr[i][j];
//             }
//         }
//         //再判断本次生成几个方块
//         //不生成的概率为20%,生成1个的概率为60%,生成2个的概率为15%,生成3个的概率为4%,生成4个的概率为1%
//         num = rand() % 100;
//         if(num < 40)
//         {
//             num = 0;
//         }
//         else if(num >= 40 && num < 100)
//         {
//             num = 1;
//         }
//         // else if(num >= 96 && num < 98)
//         // {
//         //     num = 2;
//         // }
//         // else if(num >= 98 && num < 99)
//         // {
//         //     num = 3;
//         // }
//         // else if(num == 99)
//         // {
//         //     num = 4;
//         // }
//         //先清空第一排
//         for(int i = 0; i < 4; i++)
//         {
//             blockarr[0][i].hue = 0;
//             blockarr[0][i].ifhave = false;
//         }
//         //最后判断方块生成在哪几列，并随机生成其颜色
//         for(int i = 0; i < num; i++)
//         {
//             uint8_t col = rand() % 4;
//             blockarr[0][col].hue = rand() % 360;
//             blockarr[0][col].ifhave = true;
//         }
//     }
    


//     //检测按下的按钮，判断是否在判定线上
//     //需要让它被按一次后就不会再冒红光
//     bool ifhave = false;
//     for(int i = 0; i < 4; i++)
//     {
//         ifhave = ifhave||blockarr[6][i].ifhave;
//     }
    
//     bool ifhit = true;
//     if(ifhave)
//     {
//         for(int i = 0; i < 4; i++)
//     {
//         if(blockarr[6][i].ifhave)
//         {
//             //先判断它是否已经被按下
//             if(blockarr[6][i].ifpressed)
//             {
//                 //如果已经被按下的话，就不用再检测了
//                 continue;
//             }
//             else
//             {
//                 //如果没有被按下的话，检测是否按下，如果按下，ifpressed置为true
//                 if(buttonarr[3][i].state!=Button::BUTTON_PRESSED && buttonarr[3][i].state!=Button::BUTTON_HOLD)
//                 {
//                     blockarr[6][i].unpressed_time++;
//                     if(blockarr[6][i].unpressed_time > 20)
//                     {
//                         ifhit = false;
//                     }
//                 }
//                 else
//                 {
//                     blockarr[6][i].ifpressed = true;
//                 }
//             }

            
//         }
//         else
//         {
//             if(buttonarr[0][i].state==Button::BUTTON_PRESSED || buttonarr[0][i].state==Button::BUTTON_HOLD)
//             {
//                 ifhit = false;
//             }
//         }
        
//     }
//     }
    
//     //如果在的话，背景板闪金光，如果不在的话，背景板闪红光
//     led_animation_background(RGB_Data, circle, ifhave , ifhit);

//     //最后根据方块位置和颜色，显示在LED上
//     for(int i = 1; i < 7; i++)
//     {
//         for(int j = 0; j < 4; j++)
//         {
//             if(blockarr[i-1][j].ifhave)
//             {
//                 HSV hsv;
//                 hsv.hue = blockarr[i-1][j].hue;
//                 hsv.saturation = 255;
//                 hsv.value = 20;
//                 RGB rgb = hsv2rgb(hsv);
//                 RGB_Data[i][j*2] = rgb;
//                 RGB_Data[i][j*2+1] = rgb;
//             }
//             else
//             {
//                 RGB_Data[i][j*2].r = 0;
//                 RGB_Data[i][j*2].g = 0;
//                 RGB_Data[i][j*2].b = 0;
//                 RGB_Data[i][j*2+1].r = 0;
//                 RGB_Data[i][j*2+1].g = 0;
//                 RGB_Data[i][j*2+1].b = 0;
//             }
//         }
//     }
    
// }




}