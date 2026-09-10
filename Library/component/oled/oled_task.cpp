#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "menu.h"
#include "screen.h"



extern "C"
{
    extern void oled_task(void *argument)
    {
        screen::instance().screen_init();
        
        while(true)
        {
            screen::instance().screen_run();
            vTaskDelay(pdMS_TO_TICKS(30));
        }
    }

    
    void oled_i2c_task(void *argument)
    {
        osSemaphoreAttr_t attr = {0};
        attr.name = "oled_send_sem";
        send_sem = osSemaphoreNew(1, 1, &attr);
        vTaskDelay(50);
        oled_init();
        while(true)
        {
            osSemaphoreAcquire(send_sem, osWaitForever);
            write_full_screen(oled_buffer);
        }
    }
}






