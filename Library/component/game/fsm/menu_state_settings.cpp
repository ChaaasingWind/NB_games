#include "../menu.h"


void menu::SettingsMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::SETTINGS;
    ctx->current_index = 0;
}


void menu::SettingsMenu::execute(menuctx* ctx)
{
    //ws切换选项，ad调整对应选项的大小
    auto w_state = virtual_controller::instance().controller.w.get_state();
    auto s_state = virtual_controller::instance().controller.s.get_state();
    auto a_state = virtual_controller::instance().controller.a.get_state();
    auto d_state = virtual_controller::instance().controller.d.get_state();
    static bool a_long_press = false;
    static bool d_long_press = false;

    if(w_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_index--;
    }
    else if(s_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_index++;
    }

    if(ctx->current_index >= SETTINGS_MENU_MAX_NUM)
    {
        ctx->current_index -= SETTINGS_MENU_MAX_NUM;
    }
    else if(ctx->current_index < 0)
    {
        ctx->current_index += SETTINGS_MENU_MAX_NUM;
    }



    if(d_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        if(ctx->current_index == 0)
        {
            ctx->volume += 1;
        }
        else if(ctx->current_index == 1)
        {
            ctx->rate += 1;
        }
    }
    else if(a_state.event == button::buttonevent_type::SINGLE_CLICK)
    {
        if(ctx->current_index == 0)
        {
            ctx->volume -= 1;
        }
        else if(ctx->current_index == 1)
        {
            ctx->rate -= 1;
        }
    }
    else if(d_state.event == button::buttonevent_type::LONG_PRESS_START||d_long_press)
    {
        static int count = 0;
        count++;
        if(count >= 50) // 每50ms加一点
        {
            //根据对应的index来调整不同的设置选项
            if(ctx->current_index == 0)
            {
                ctx->volume += 1;
            }
            else if(ctx->current_index == 1)
            {
                ctx->rate += 1;
            }
            count = 0;
        }
        d_long_press = true;
    }
    else if(a_state.event == button::buttonevent_type::LONG_PRESS_START||a_long_press)
    {
        static int count = 0;
        count++;
        if(count >= 50) // 每50ms减一点
        {
            //根据对应的index来调整不同的设置选项
            if(ctx->current_index == 0)
            {
                ctx->volume -= 1;
            }
            else if(ctx->current_index == 1)
            {
                ctx->rate -= 1;
            }
            count = 0;
        }
        a_long_press = true;
    }


    if(d_state.event == button::buttonevent_type::LONG_PRESS_END)
    {
        d_long_press = false;
    }
    if(a_state.event == button::buttonevent_type::LONG_PRESS_END)
    {
        a_long_press = false;
    }
    

    //限幅
    if(ctx->rate >= 100)
    {
        ctx->rate = 100;
    }
    else if(ctx->rate <= 0)
    {
        ctx->rate = 0;
    }
    if(ctx->volume >= 100)
    {
        ctx->volume = 100;
    }
    else if(ctx->volume <= 0)
    {
        ctx->volume = 0;
    }
    
    if(virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._mainMenu);
    }
}