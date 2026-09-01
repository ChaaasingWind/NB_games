#include "../menu.h"


void menu::MainMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::MAIN;
    ctx->current_index = 0;
}


void menu::MainMenu::execute(menuctx* ctx)
{
    if(virtual_controller::instance().controller.z.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch((instance().MainMenuOptions[ctx->current_index]));
        return;
    }


    if(virtual_controller::instance().controller.w.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_index--;
    }
    else if(virtual_controller::instance().controller.s.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_index++;
    }


    if(ctx->current_index >= MAIN_MENU_MAX_NUM)
    {
        ctx->current_index -= MAIN_MENU_MAX_NUM;
    }
    else if(ctx->current_index < 0)
    {
        ctx->current_index += MAIN_MENU_MAX_NUM;
    }

    
}