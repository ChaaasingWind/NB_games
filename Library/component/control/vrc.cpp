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