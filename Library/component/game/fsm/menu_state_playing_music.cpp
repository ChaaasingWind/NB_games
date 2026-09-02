#include "../menu.h"


void menu::PlayingMusicMenu::enter(menuctx* ctx)
{
    ctx->current_state = MenuState::PLAYING_MUSIC;
    ctx->current_playing_state = MusicPlayingState::PLAYING;
    

    //如果是选择了已经播放结束的歌曲，则重置该歌曲
    if(ctx->current_music_index >= 0 && music_play::instance().song_finished)
    {
        music_play::instance().set_same_song();
    }

    ctx->current_music_index = ctx->current_index;
}


void menu::PlayingMusicMenu::execute(menuctx* ctx)
{
    if (virtual_controller::instance().controller.z.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        if(ctx->current_playing_state == MusicPlayingState::STOP)
        {
            // 如果当前播放结束的歌曲，则根据当前的播放模式来决定下一首歌的播放
            if(music_play::instance().song_finished)
            {
                if(ctx->_playing_mode == MusicPlayingMode::CIRCLE || ctx->_playing_mode == MusicPlayingMode::ONCE)
                {
                    //单曲循环
                    music_play::instance().set_same_song();
                }
                else if(ctx->_playing_mode == MusicPlayingMode::SEQUENTIAL)
                {
                    ctx->current_music_index = (ctx->current_music_index + 1) % MUSIC_MENU_MAX_NUM;
                }
                else if(ctx->_playing_mode == MusicPlayingMode::RANDOM)
                {
                    ctx->current_music_index = HAL_GetTick() % MUSIC_MENU_MAX_NUM;
                }
            }
            ctx->current_playing_state = MusicPlayingState::PLAYING;
        }
        else if(ctx->current_playing_state == MusicPlayingState::PLAYING)
        {
            ctx->current_playing_state = MusicPlayingState::STOP;
        }
        
    }
    else if(virtual_controller::instance().controller.x.get_state().event == button::buttonevent_type::SINGLE_CLICK)
        {
            ctx->_playing_mode = static_cast<MusicPlayingMode>((static_cast<int>(ctx->_playing_mode) + 1) % 4);
        }
    else if (virtual_controller::instance().controller.c.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        request_switch(&instance()._musicMenu);
        return;
    }

    if(virtual_controller::instance().controller.a.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_music_index--;
    }
    else if(virtual_controller::instance().controller.d.get_state().event == button::buttonevent_type::SINGLE_CLICK)
    {
        ctx->current_music_index++;
    }
    
}