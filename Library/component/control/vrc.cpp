#include "vrc.h"

__attribute__((section(".sram2"))) uart_connect_pack virtual_controller::rxbuffer;

void virtual_controller::update_feedback()
{
    controller.w = rxbuffer.msg.w;
    controller.a = rxbuffer.msg.a;
    controller.s = rxbuffer.msg.s;
    controller.d = rxbuffer.msg.d;
    controller.z = rxbuffer.msg.z;
    controller.x = rxbuffer.msg.x;
    controller.c = rxbuffer.msg.c;
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart1)
    {
        virtual_controller::instance().update_feedback();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&virtual_controller::instance().rxbuffer, sizeof(uart_connect_pack));
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // 清除所有错误标志
        __HAL_UART_CLEAR_FLAG(huart, UART_CLEAR_PEF | UART_CLEAR_FEF | 
                                      UART_CLEAR_NEF | UART_CLEAR_OREF);
        // 重新启动接收...
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, (uint8_t*)&virtual_controller::instance().rxbuffer, sizeof(uart_connect_pack));
    }
}