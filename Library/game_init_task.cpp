#include "FreeRtos.h"
#include "task.h"
#include "vrc.h"









extern "C"
{
    void game_init_task(void *argument)
    {
        SCB_EnableICache();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&(virtual_controller::instance().rxbuffer), sizeof(virtual_controller::instance().rxbuffer));

        vTaskDelete(NULL);
    }

}