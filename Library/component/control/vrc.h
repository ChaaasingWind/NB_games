#include "component/oled/oled.h"
#include "main.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal_uart.h"
#include "task.h"
#include "string.h"
#include "data_def.h"
#include "blackboard.h"
#include <type_traits>

extern UART_HandleTypeDef huart1;

// class button
// {
//     enum class buttonstate
//     {
//         UP,
//         DOWN,
//     };
//     enum class buttonevent
//     {
//         SINGLE_CLICK,
//         LONG_PRESS_START,
//         LONG_PRESS_END,
//     };
// }

class virtual_controller
{
  public:

    struct axes
    {
        float x;
        float y;
        bool enter;
    };

    struct finalcontroller
    {
        bool w;
        bool a;
        bool s;
        bool d;
        bool z;
        bool x;
        bool c;
        axes axes1;
        axes axes2;
    };

    static virtual_controller& instance()
    {
        static virtual_controller instance;
        return instance;
    }
    void update_feedback();
    finalcontroller controller;
    __attribute__((section(".sram2"))) static uart_connect_pack rxbuffer; // 接收缓冲区
};





