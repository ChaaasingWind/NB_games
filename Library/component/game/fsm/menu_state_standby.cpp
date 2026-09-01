#include "../menu.h"


void menu::StandbyMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::STANDBY;
    ctx->current_index = 0;
}


void menu::StandbyMenu::execute(menuctx* ctx)
{
    if(virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._mainMenu);
        return;
    }
}