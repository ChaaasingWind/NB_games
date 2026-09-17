#include "FreeRTOS.h"
#include "task.h"
#include "w25q64.h"
#include <string.h>

#define FLASH_WRITE_MODE 1

extern OSPI_HandleTypeDef hospi1;



extern "C"
{


    void flash_task(void *argument)
    {
        w25q64& flash = w25q64::instance(&hospi1);
        flash.w25q64_init();
    

        while(true)
        {

            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }





}