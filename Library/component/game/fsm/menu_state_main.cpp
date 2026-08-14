#include "../menu.h"


void menu::MainMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::MAIN;
    ctx->current_index = 0;
}


void menu::MainMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == blue_tooth_cmd::YES)
    {
        request_switch(&instance()._musicMenu);
        return;
    }


    if(ctx->cmd == blue_tooth_cmd::UP)
    {
        ctx->current_index++;
    }
    else if(ctx->cmd == blue_tooth_cmd::DOWN)
    {
        ctx->current_index--;
    }

    
}