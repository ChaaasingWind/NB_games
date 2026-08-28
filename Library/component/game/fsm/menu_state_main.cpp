#include "../menu.h"


void menu::MainMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::MAIN;
    ctx->current_index = 0;
}


void menu::MainMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == controller_cmd::YES)
    {
        request_switch((instance().MainMenuOptions[ctx->current_index]));
        return;
    }


    if(ctx->cmd == controller_cmd::UP)
    {
        ctx->current_index--;
    }
    else if(ctx->cmd == controller_cmd::DOWN)
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