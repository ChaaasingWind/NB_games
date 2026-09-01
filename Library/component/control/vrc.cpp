#include "vrc.h"

__attribute__((section(".sram2"))) uart_connect_pack virtual_controller::rxbuffer;

button::button_data debug;
uint32_t debug_time = 0;

void button::update()
{
    uint32_t current_time = HAL_GetTick();
    if (_data.state == button_state::LOW)
    {
        if (button_value == 1)
        {
            if(current_time - last_value_change_time >= 10)
            {
                _data.edge = edge_type::RISING;
                _data.state = button_state::HIGH;
                rising_edge_detect_time = current_time;
                last_value_change_time = current_time;
            }
            else
            {
                _data.edge = edge_type::NONE;
            }
        }
        else
        {
            _data.edge = edge_type::NONE;
            last_value_change_time = current_time;
        }
        _data.event = buttonevent_type::NONE;
    }
    else if (_data.state == button_state::HIGH)
    {
        if (button_value == 0)
        {
            if(current_time - last_value_change_time >= 10)
            {
                _data.edge = edge_type::FALLING;
                _data.state = button_state::LOW;
                long_start_send = false;
                last_value_change_time = current_time;

                //event事件更新
                if(current_time - rising_edge_detect_time >= 500)
                {
                    _data.event = buttonevent_type::LONG_PRESS_END;
                }
                else
                {
                    _data.event = buttonevent_type::SINGLE_CLICK;
                }
            }
            else
            {
                _data.edge = edge_type::NONE;
                _data.event = buttonevent_type::NONE;
            }
        }
        else
        {
            if(current_time - rising_edge_detect_time >= 500 && !long_start_send)
            {
                _data.event = buttonevent_type::LONG_PRESS_START;
                long_start_send = true;
            }
            else
            {
                _data.event = buttonevent_type::NONE;
            }
            last_value_change_time = current_time;
            _data.edge = edge_type::NONE;
        }

    }
}


button::button_data button::get_state()
{
    button_data result = _data;
    _data.event = buttonevent_type::NONE;
    _data.edge = edge_type::NONE;
    return result;
    return _data;
}


void virtual_controller::update_feedback()
{
    controller.w.button_value = rxbuffer.msg.w;
    controller.a.button_value = rxbuffer.msg.a;
    controller.s.button_value = rxbuffer.msg.s;
    controller.d.button_value = rxbuffer.msg.d;
    controller.z.button_value = rxbuffer.msg.z;
    controller.x.button_value = rxbuffer.msg.x;
    controller.c.button_value = rxbuffer.msg.c;

    controller.w.update();
    controller.a.update();
    controller.s.update();
    controller.d.update();
    controller.z.update();
    controller.x.update();
    controller.c.update();

    debug = controller.w._data;
}



void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if(huart == &huart1)
    {
        if(Size == sizeof(uart_connect_pack))
        {
            virtual_controller::instance().update_feedback();
        }
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