#include "../menu.h"

#define MUSIC_MENU_MAX_INDEX 10

void menu::MusicMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::MUSIC;
    ctx->current_index = 0;
}


void menu::MusicMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == blue_tooth_cmd::RETURN)
    {
        request_switch(&instance()._mainMenu);
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

    if(ctx->current_index >= MUSIC_MENU_MAX_INDEX)
    {
        ctx->current_index -= MUSIC_MENU_MAX_INDEX;
    }
    else if(ctx->current_index < 0)
    {
        ctx->current_index += MUSIC_MENU_MAX_INDEX;
    }

    
}