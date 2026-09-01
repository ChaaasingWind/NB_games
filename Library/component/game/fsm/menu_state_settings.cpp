#include "../menu.h"


void menu::SettingsMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::SETTINGS;
    ctx->current_index = 0;
}


void menu::SettingsMenu::execute(menuctx* ctx)
{
    auto w_state = virtual_controller::instance().controller.w.get_state();
    auto s_state = virtual_controller::instance().controller.s.get_state();
    static bool w_long_press = false;
    static bool s_long_press = false;

    if(w_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->volume += 1;
    }
    else if(s_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->volume -= 1;
    }
    else if(w_state.event == button::buttonevent_type::LONG_PRESS_START||w_long_press)
    {
        static int count = 0;
        count++;
        if(count >= 50) // 每50ms加一点
        {
            ctx->volume += 1;
            count = 0;
        }
        w_long_press = true;
    }
    else if(s_state.event == button::buttonevent_type::LONG_PRESS_START||s_long_press)
    {
        static int count = 0;
        count++;
        if(count >= 50) // 每50ms减一点
        {
            ctx->volume -= 1;
            count = 0;
        }
        s_long_press = true;
    }


    if(w_state.event == button::buttonevent_type::LONG_PRESS_END)
    {
        w_long_press = false;
    }
    if(s_state.event == button::buttonevent_type::LONG_PRESS_END)
    {
        s_long_press = false;
    }
    



    //限幅
    if(ctx->volume >= 100)
    {
        ctx->volume = 100;
    }
    if(ctx->volume <= 0)
    {
        ctx->volume = 0;
    }
    
    if(virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._mainMenu);
    }
}