#include "main.h"
#include "FreeRTOS.h"
#include "task.h"




extern "C" 
{
void heartbeat_task(void *argument)
{
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_7);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
}