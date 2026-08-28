#include "FreeRtos.h"
#include "task.h"
#include "vrc.h"










extern "C"
{
    void game_init_task(void *argument)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&(virtual_controller::instance().rxbuffer), sizeof(uart_connect_pack));

        vTaskDelete(NULL);
    }

}