#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "../led/led_animation.h"
#include "stdlib.h"



extern "C"
{
    
    void game_task(void *pvParameters)
    {
    
        
        while(1)
        {
            
            
            vTaskDelay(1);
        }
    }  
}