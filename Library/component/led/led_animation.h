#ifndef LED_TASK_H

#define LED_TASK_H

#include "main.h"
#include <stdint.h>
#include "cmsis_os.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "tim.h"
#include "dma.h"
#include <charconv>
#include <cstdint>
#include <cstring>
#include <cmath>




typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;



typedef struct
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
} HSV;



void fill(RGB RGB_Data[8][8] , uint16_t PWM_Buffer[64*24+64]);

void Send(uint16_t PWM_Buffer[64*24+64] , SemaphoreHandle_t dmaSemaphore);

RGB hsv2rgb(HSV hsv);

void led_clear(RGB led[8][8]);

void pattern_mask(RGB led[8][8], const bool mask[8][8]);

void buffer_clear(uint16_t buffer[64*24+64]);

void led_animation1(RGB led[8][8] , int circle);

void led_animation2(RGB led[8][8] , int circle);

void led_animation3(RGB led[8][8] , int circle);


//游戏背景及判定线显示
void led_animation_background(RGB led[8][8] , int circle , bool iffree , bool ifhit);

void led_mask_animation1(RGB RGB_Data[8][8]);

void led_mask_animation2(RGB RGB_Data[8][8], int circle);

void led_mask_animation3(RGB RGB_Data[8][8], int circle);

const bool pattern_empty[8][8] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

const bool character_Lu[8][8] = {
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 0, 0, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0}
};

const bool character_Xiang[8][8] = {
    {0, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 0, 0, 1},
    {0, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 0, 0, 1},
    {1, 1, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 0, 1, 1, 1, 1},
    {0, 1, 0, 0, 1, 0, 0, 1},
    {0, 1, 0, 0, 1, 1, 1, 1}
};

const bool character_Ze[8][8] = {
    {1, 0, 0, 1, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 0, 1, 1, 0},
    {1, 0, 0, 1, 1, 1, 0, 0},
    {1, 0, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
};

const bool word_NB[8][8] = {
    {1, 0, 0, 1, 1, 1, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 1, 1, 0},
    {1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 0}
};

const bool character_Zhang[8][8] = {
    {1, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 1, 1, 0, 0, 0},
    {1, 1, 0, 1, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 0, 1, 1, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 1, 0},
    {1, 1, 0, 1, 1, 0, 0, 1}
};

const bool character_Zi[8][8] = {
    {0, 1, 0, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 0, 0, 1, 0, 1, 0},
    {1, 1, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 1, 1, 1, 1},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0}
};
const bool character_Hao[8][8] = {
    {0, 0, 0, 1, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 0, 0, 1, 0, 0, 1, 0},
    {0, 1, 1, 0, 0, 1, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {1, 1, 0, 1, 0, 1, 1, 0}
};
const bool character_Sheng[8][8] = {
    {0, 1, 0, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1}
};
const bool character_Ri[8][8] = {
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1}
};
const bool character_Kuai[8][8]
{
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0, 0, 0, 1}
};
const bool character_Le[8][8]
{
    {0, 0, 0, 0, 1, 1, 1, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 0, 0},
    {0, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 1, 0, 0, 1}
};

#endif // LED_TASK_H