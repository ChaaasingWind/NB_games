#include "../menu.h"



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

    if(ctx->cmd == blue_tooth_cmd::YES)
    {
        request_switch(&instance()._playingMusicMenu);
    }

    if(ctx->cmd == blue_tooth_cmd::UP)
    {
        ctx->current_index--;
    }
    else if(ctx->cmd == blue_tooth_cmd::DOWN)
    {
        ctx->current_index++;
    }

    if(ctx->current_index >= MUSIC_MENU_MAX_NUM)
    {
        ctx->current_index -= MUSIC_MENU_MAX_NUM;
    }
    else if(ctx->current_index < 0)
    {
        ctx->current_index += MUSIC_MENU_MAX_NUM;
    }

    
}