#include "component/oled/oled.h"
#include "main.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal_uart.h"
#include "task.h"
#include "string.h"
#include <type_traits>

extern UART_HandleTypeDef huart1;


struct __attribute__((packed)) uart_cmd
{
    uint8_t w  : 1  ;
    uint8_t a  : 1  ;
    uint8_t s  : 1  ;
    uint8_t d  : 1  ;
    uint8_t z  : 1  ;
    uint8_t x  : 1  ;
    uint8_t c  : 1  ;
};

struct uart_connect_pack
{
    uint8_t head;
    uart_cmd msg;
    uint8_t check;
    uint8_t tail;
};

class button
{
  
  public:
    enum class edge_type
    {
        RISING,
        FALLING,
        NONE
    };
    enum class button_state
    {
        LOW,
        HIGH,
    };
    enum class buttonevent_type
    {
        SINGLE_CLICK,
        LONG_PRESS_START,
        LONG_PRESS_END,
        NONE,
    };
    struct button_data
    {
        edge_type edge         = edge_type::NONE;
        buttonevent_type event = buttonevent_type::NONE;
        button_state state     = button_state::LOW;
    };

  private:
    
    
    bool long_start_send = false;
    uint32_t last_value_change_time = 0;
    uint32_t rising_edge_detect_time = 0;
    
    public:
    button_data _data;
    bool button_value;
    void update();
    button_data get_state();
};

class virtual_controller
{
  public:

    struct axes
    {
        float x;
        float y;
        button enter;
    };

    struct finalcontroller
    {
        button w;
        button a;
        button s;
        button d;
        button z;
        button x;
        button c;
        axes axes1;
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





