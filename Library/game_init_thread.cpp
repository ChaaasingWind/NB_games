#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "stm32_hal_legacy.h"
#include "FreeRTOS.h"
#include "task.h"


//#define BUTTON_TASK 1
#define GAME_TASK 0
#define LED_TASK 0
#define BUZZER_TASK 1
#define OLED_TASK 1
#define HEARTBEAT_TASK 1
#define BLUETOOTH_TASK 1





extern "C"
{
//extern void button_group_task(void *argument);
extern void game_task(void *argument);
extern void led_task(void *argument);
extern void buzzer_task(void *argument);
extern void oled_i2c_task(void *argument);
extern void oled_task(void *argument);
extern void heartbeat_task(void *argument);
extern void bluetooth_task(void *argument);



void StartDefaultTask(void *argument)
{
    // #if BUTTON_TASK
    //     xTaskCreate(button_group_task, "button_group_task", 64, NULL,
    //                 configMAX_PRIORITIES - 1, NULL);
    //     #endif
    #if GAME_TASK
        xTaskCreate(game_task, "game_task", 128, NULL,configMAX_PRIORITIES - 1, NULL);
        #endif
    #if LED_TASK
        xTaskCreate(led_task, "led_task", 256, NULL,configMAX_PRIORITIES - 1, NULL);
        #endif
    #if BUZZER_TASK
        xTaskCreate(buzzer_task, "buzzer_task", 256, NULL,configMAX_PRIORITIES - 1, NULL);
        #endif
    #if OLED_TASK
        xTaskCreate(oled_i2c_task, "oled_i2c_task", 256, NULL,configMAX_PRIORITIES - 2, NULL);
        xTaskCreate(oled_task, "oled_task", 256, NULL,configMAX_PRIORITIES - 2, NULL);
        #endif
    #if HEARTBEAT_TASK
        xTaskCreate(heartbeat_task, "heartbeat_task", 64, NULL,configMAX_PRIORITIES - 4, NULL);
    #endif
    #if BLUETOOTH_TASK
        xTaskCreate(bluetooth_task, "bluetooth_task", 256, NULL,configMAX_PRIORITIES - 3, NULL);
    #endif
    vTaskDelete(NULL);
}



}