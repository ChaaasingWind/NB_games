#include "main.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal_uart.h"
#include "task.h"
#include "string.h"
#include "../../data_def.h"
#include "../../Blackboard/blackboard.h"

extern UART_HandleTypeDef huart1;
__attribute__((section(".sram2"))) uart_connect_pack rxbuffer; // 接收缓冲区

extern "C" 
{
    void bluetooth_task(void *argument)
    {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&rxbuffer, sizeof(uart_connect_pack));
        while(1)
        {
            vTaskDelay(1000);
        }
    }

    void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
    {
        if(huart == &huart1)
        {
            blackboard::instance().write(rxbuffer.msg);
            HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&rxbuffer, sizeof(uart_connect_pack));
        }
    }
}