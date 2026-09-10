#include "screen.h"

void screen::PlayingMusicScreen::enter(screen_ctx* ctx)
{
    
}

void screen::PlayingMusicScreen::execute(screen_ctx* ctx)
{
    //顶部写歌名
    pen::instance()
    .set_position(0, 0)
    .draw_string(song_name_list[ctx->current_music_index], 0, 2);
    if(ctx->current_playing_state == menu::MusicPlayingState::PLAYING)
    {
        pen::instance()
        .set_position(34, 40)
        .draw_string("Playing...");
    }
    else if(ctx->current_playing_state == menu::MusicPlayingState::STOP)
    {
        pen::instance()
        .set_position(43, 40)
        .draw_string("S.T.O.P");
    }


    pen::instance()
    .set_position(10, 50);
    if(ctx->_playing_mode == menu::MusicPlayingMode::CIRCLE)
    {
        pen::instance().draw_string("MODE : CIRCLE", 0, 2);
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::ONCE)
    {
        pen::instance().draw_string("MODE : ONCE", 0, 2);
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::SEQUENTIAL)
    {
        pen::instance().draw_string("MODE : SEQUENTIAL", 0, 2);
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::RANDOM)
    {
        pen::instance().draw_string("MODE : RANDOM", 0, 2);
    }
}