#include "../menu.h"


void menu::PlayingMusicMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::PLAYING_MUSIC;
    ctx->current_playing_state = MusicPlayingState::PLAYING;
    ctx->current_music_index = ctx->current_index;
}


void menu::PlayingMusicMenu::execute(menuctx* ctx)
{
    if(ctx->cmd == blue_tooth_cmd::PAUSE)
    {
        ctx->current_playing_state = MusicPlayingState::STOP;
    }
    else if (ctx->cmd == blue_tooth_cmd::YES)
    {
        ctx->current_playing_state = MusicPlayingState::PLAYING;
    }
    else if (ctx->cmd == blue_tooth_cmd::RETURN)
    {
        request_switch(&instance()._musicMenu);
        return;
    }

    
}