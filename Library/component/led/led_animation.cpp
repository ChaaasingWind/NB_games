#include "led_animation.h"







#define CODE_0 110
#define CODE_1 233
#define REAL_ZERO 0

extern TIM_HandleTypeDef htim3;
extern DMA_HandleTypeDef hdma_tim3_ch1;


void fill(RGB RGB_Data[8][8] , uint16_t PWM_Buffer[64*24+64])
{
    for (int i = 0; i < 8; i++)
        {
            if(i%2 == 1)
            {
                for (int j = 0; j < 8; j++)
                {
                    for(int k = 0; k < 3; k++)
                    {
                        RGB rgb = RGB_Data[i][j];
                        if(k == 0)
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + j ) * 24 + 8 + (7-l)] = (rgb.r >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                        else if(k == 1)
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + j ) * 24 + (7-l)] = (rgb.g >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                        else
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + j ) * 24 + 16 + (7-l)] = (rgb.b >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                    }
                }
            }
            else
            {
                for (int j = 7; j >= 0; j--)
                {
                    for(int k = 0; k < 3; k++)
                    {
                        RGB rgb = RGB_Data[i][j];
                        if(k == 0)
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + (7 - j) ) * 24 + 8 + (7-l)] = (rgb.r >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                        else if(k == 1)
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + (7 - j) ) * 24 + (7-l)] = (rgb.g >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                        else
                        {
                            for (int l = 7; l >= 0; l--)
                            {
                                PWM_Buffer[(i * 8 + (7 - j) ) * 24 + 16 + (7-l)] = (rgb.b >> l) & 1 ? CODE_1 : CODE_0;
                            }
                        }
                    }
                }
            }
            
            
        }
    

}

void Send(uint16_t PWM_Buffer[64*24+64] , SemaphoreHandle_t dmaSemaphore)
{
    
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_1, (uint32_t*)PWM_Buffer, (64 * 24+64));
    if (xSemaphoreTake(dmaSemaphore, pdMS_TO_TICKS(10)) == pdTRUE)
    {
        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    }
    else
    {
        // 超时处理
        HAL_TIM_PWM_Stop_DMA(&htim3, TIM_CHANNEL_1);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    }
}


RGB hsv2rgb(HSV hsv) {
    RGB rgb;
    uint16_t hue = hsv.hue % 360;
    uint8_t sat = hsv.saturation;
    uint8_t val = hsv.value;

    // 饱和度为0，直接返回灰度
    if (sat == 0) {
        rgb.r = val;
        rgb.g = val;
        rgb.b = val;
        return rgb;
    }

    uint8_t region = hue / 60;          // 0~5
    uint8_t remainder = hue % 60;       // 0~59

    // 中间变量，使用16位避免乘法溢出
    uint16_t p = (uint16_t)val * (255 - sat) / 255;
    uint16_t q = (uint16_t)val * (255 - (sat * remainder) / 60) / 255;
    uint16_t t = (uint16_t)val * (255 - (sat * (60 - remainder)) / 60) / 255;

    switch (region) {
        case 0:
            rgb.r = val;
            rgb.g = t;
            rgb.b = p;
            break;
        case 1:
            rgb.r = q;
            rgb.g = val;
            rgb.b = p;
            break;
        case 2:
            rgb.r = p;
            rgb.g = val;
            rgb.b = t;
            break;
        case 3:
            rgb.r = p;
            rgb.g = q;
            rgb.b = val;
            break;
        case 4:
            rgb.r = t;
            rgb.g = p;
            rgb.b = val;
            break;
        default:  // case 5
            rgb.r = val;
            rgb.g = p;
            rgb.b = q;
            break;
    }

    return rgb;
}


void led_clear(RGB led[8][8])
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            led[i][j].r = 0;
            led[i][j].g = 0;
            led[i][j].b = 0;
        }
    }
}

void buffer_clear(uint16_t buffer[64*24+64])
{
    for(int i = 0; i < 64*24+64; i++)
    {
        buffer[i] = REAL_ZERO;
    }
}

//横向动态彩色流动效果
void led_animation1(RGB led[8][8], int circle)
{
    for(int i = 0; i < 8; i++)
    {
        HSV hsv;
        hsv.hue = (circle*3 + i*10) % 360;
            hsv.saturation = 255;
            hsv.value = 10;
        for(int j = 0; j < 8; j++)
        {
            led[j][7-i] = hsv2rgb(hsv);
        }
    }
}

//对角向动态彩色流动效果
void led_animation2(RGB led[8][8], int circle)
{
    for(int i = 0; i < 8; i++)
    {
        
        for(int j = 0; j < 8; j++)
        {
            HSV hsv;
            hsv.hue = (circle*3 + i*10 + j*10) % 360;
            hsv.saturation = 255;
            hsv.value = 10;
            led[j][i] = hsv2rgb(hsv);
        }
    }
}

//中心向外动态彩色流动效果
void led_animation3(RGB led[8][8], int circle)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            int distancei = i-3.5 >=0 ? i-3.5 : 3.5-i;
            int distancej = j-3.5 >=0 ? j-3.5 : 3.5-j;
            HSV hsv;
            hsv.hue = (circle*5 + (distancei+distancej)*20) % 360;
            hsv.saturation = 255;
            hsv.value = 10;
            led[j][i] = hsv2rgb(hsv);
        }
    }
}




void led_animation_background(RGB led[8][8] , int circle , bool iffree , bool ifhit)
{
    if(iffree)
    {
        if(ifhit)
        {
            for(int i = 0; i < 8; i++)
            {
                HSV hsv;
                hsv.hue = (circle*3 + i*10) % 360;
                hsv.saturation = 255;
                hsv.value = 10;
                led[0][7-i] = hsv2rgb(hsv);
                hsv.hue = 50;
                hsv.value = 20;
                led[7][i] = hsv2rgb(hsv);
                
            }
        }
        else 
        {
            for(int i = 0; i < 8; i++)
            {
                HSV hsv;
                hsv.hue = (circle*3 + i*10) % 360;
                hsv.saturation = 255;
                hsv.value = 10;
                led[0][7-i] = hsv2rgb(hsv);
                hsv.hue = 0;
                hsv.value = 20;
                led[7][i] = hsv2rgb(hsv);
            }
        }
        
    }
    else
    {
        for(int i = 0; i < 8; i++)
        {
            HSV hsv;
            hsv.hue = (circle*3 + i*10) % 360;
                hsv.saturation = 255;
                hsv.value = 10;
                led[0][7-i] = hsv2rgb(hsv);

            led[7][i].r = 10;
            led[7][i].g = 10;
            led[7][i].b = 10;

        }

    }
    
}

void pattern_mask(RGB led[8][8], const bool mask[8][8])
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            if(!mask[i][j])
            {
                led[i][j].r = 0;
                led[i][j].g = 0;
                led[i][j].b = 0;
            }
        }
    }
}

// //这个动画会根据按钮按下来显示不同的字
// void led_mask_animation1(RGB RGB_Data[8][8])
// {
//     int pattern_index = -1;

//         if(buttonarr[0][0].state == Button::BUTTON_PRESSED || buttonarr[0][0].state == Button::BUTTON_HOLD)
//         {
//             pattern_index = 0;
//         }
//         else if(buttonarr[0][1].state == Button::BUTTON_PRESSED || buttonarr[0][1].state == Button::BUTTON_HOLD)
//         {
//             pattern_index = 1;
//         }
//         else if(buttonarr[0][2].state == Button::BUTTON_PRESSED || buttonarr[0][2].state == Button::BUTTON_HOLD)
//         {
//             pattern_index = 2;
//         }
//         else if(buttonarr[0][3].state == Button::BUTTON_PRESSED || buttonarr[0][3].state == Button::BUTTON_HOLD)
//         {
//             pattern_index = 3;
//         }
//         else if(buttonarr[1][0].state == Button::BUTTON_PRESSED || buttonarr[1][0].state == Button::BUTTON_HOLD)
//         {
//             pattern_index = 4;
//         }


//         if(pattern_index == 0)
//         {
//             pattern_mask(RGB_Data, character_Lu);
//         }
//         else if(pattern_index == 1)
//         {
//             pattern_mask(RGB_Data, character_Xiang);
//         }
//         else if(pattern_index == 2)
//         {
//             pattern_mask(RGB_Data, character_Ze);
//         }
//         else if(pattern_index == 3)
//         {
//             pattern_mask(RGB_Data, word_NB);
//         }
//         else if(pattern_index == -1)
//         {
//             pattern_mask(RGB_Data, pattern_empty);
//         }
// }
//这个动画会循环演示“卢相泽NB”这几个字
void led_mask_animation2(RGB RGB_Data[8][8], int circle)
{
    int pattern_index = circle/100 % 4;

        if(pattern_index == 0)
        {
            pattern_mask(RGB_Data, character_Lu);
        }
        else if(pattern_index == 1)
        {
            pattern_mask(RGB_Data, character_Xiang);
        }
        else if(pattern_index == 2)
        {
            pattern_mask(RGB_Data, character_Ze);
        }
        else if(pattern_index == 3)
        {
            pattern_mask(RGB_Data, word_NB);
        }
        
}

//循环显示“张梓豪快乐”
void led_mask_animation3(RGB RGB_Data[8][8], int circle)
{
    int pattern_index = circle/100 % 7;

        if(pattern_index == 0)
        {
            pattern_mask(RGB_Data, character_Zhang);
        }
        else if(pattern_index == 1)
        {
            pattern_mask(RGB_Data, character_Zi);
        }
        else if(pattern_index == 2)
        {
            pattern_mask(RGB_Data, character_Hao);
        }
        else if(pattern_index == 3)
        {
            pattern_mask(RGB_Data, character_Sheng);
        }
        else if(pattern_index == 4)
        {
            pattern_mask(RGB_Data, character_Ri);
        }
        else if(pattern_index == 5)
        {
            pattern_mask(RGB_Data, character_Kuai);
        }
        else if(pattern_index == 6)
        {
            pattern_mask(RGB_Data, character_Le);
        }
}
