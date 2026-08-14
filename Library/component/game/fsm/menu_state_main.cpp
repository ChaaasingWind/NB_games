#include "../menu.h"

#define MAIN_MENU_MAX_INDEX 2
void menu::MainMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::MAIN;
    ctx->current_index = 0;
}


void menu::MainMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == blue_tooth_cmd::YES)
    {
        request_switch((instance().MainMenuOptions[ctx->current_index]));
        return;
    }


    if(ctx->cmd == blue_tooth_cmd::UP)
    {
        ctx->current_index--;
    }
    else if(ctx->cmd == blue_tooth_cmd::DOWN)
    {
        ctx->current_index++;
    }


    if(ctx->current_index >= MAIN_MENU_MAX_INDEX)
    {
        ctx->current_index -= MAIN_MENU_MAX_INDEX;
    }
    else if(ctx->current_index < 0)
    {
        ctx->current_index += MAIN_MENU_MAX_INDEX;
    }

    
}