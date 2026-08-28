#include "../menu.h"


void menu::StandbyMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::STANDBY;
    ctx->current_index = 0;
}


void menu::StandbyMenu::execute(menuctx* ctx)
{
    if(ctx->cmd != controller_cmd::NONE)
    {
        request_switch(&instance()._mainMenu);
        return;
    }
}