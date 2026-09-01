#include "../menu.h"


void menu::PlayingMusicMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::PLAYING_MUSIC;
    ctx->current_playing_state = MusicPlayingState::PLAYING;
    ctx->current_music_index = ctx->current_index;
}


void menu::PlayingMusicMenu::execute(menuctx* ctx)
{
    if (virtual_controller::instance().controller.z.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        if(ctx->current_playing_state == MusicPlayingState::STOP)
        {
            ctx->current_playing_state = MusicPlayingState::PLAYING;
            ctx->current_music_index = ctx->current_index;
        }
        else if(ctx->current_playing_state == MusicPlayingState::PLAYING)
        {
            ctx->current_playing_state = MusicPlayingState::STOP;
            
        }
        
    }
    else if(virtual_controller::instance().controller.x.get_state().event == button::buttonevent_type::SINGLE_CLICK)
        {
            ctx->music_is_looped =! ctx->music_is_looped;
        }
    else if (virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._musicMenu);
        return;
    }

    
}