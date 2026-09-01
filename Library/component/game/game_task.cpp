#include "main.h"
#include "gpio.h"
#include "stdlib.h"
#include "menu.h"
#include "FreeRTOS.h"
#include "task.h"



extern "C"
{
    
    void game_task(void *pvParameters)
    {
        menu::instance().menu_init();
        while(1)
        {
            
            menu::instance().menu_run();

            vTaskDelay(1);
        }
    }  
}