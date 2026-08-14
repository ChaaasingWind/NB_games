#include "main.h"
#include "cmsis_os.h"
#include "gpio.h"
#include "../led/led_animation.h"
#include "stdlib.h"
#include "menu.h"
#include "../../Blackboard/blackboard.h"



extern "C"
{
    
    void game_task(void *pvParameters)
    {
        menu::instance().menu_init();
        while(1)
        {
            menu::instance().menu_run();
            blackboard::instance().write(menu::instance().read_ctx());

            vTaskDelay(1);
        }
    }  
}