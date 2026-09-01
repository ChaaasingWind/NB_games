#include "../menu.h"


void menu::DebugMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::_DEBUG;
    ctx->current_index = 0;
}


void menu::DebugMenu::execute(menuctx* ctx)
{
    if(virtual_controller::instance().controller.w.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->volume -= 0.01f;
    }
    else if(virtual_controller::instance().controller.s.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->volume += 0.01f;
    }
    if(ctx->volume >= 1.0f)
    {
        ctx->volume = 1.0f;
    }
    if(ctx->volume <= 0.0f)
    {
        ctx->volume = 0.0f;
    }
    
    if(virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._mainMenu);
    }
}