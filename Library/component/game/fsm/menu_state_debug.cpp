#include "../menu.h"
#include "data_def.h"


void menu::DebugMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::_DEBUG;
    ctx->current_index = 0;
}


void menu::DebugMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == controller_cmd::UP)
    {
        ctx->current_index--;
    }
    else if(ctx->cmd == controller_cmd::DOWN)
    {
        ctx->current_index++;
    }
    if(ctx->cmd == controller_cmd::RETURN)
    {
        request_switch(&instance()._mainMenu);
    }
}