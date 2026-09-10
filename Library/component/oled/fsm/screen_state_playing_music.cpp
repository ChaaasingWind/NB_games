#include "screen.h"

void screen::PlayingMusicScreen::enter(screen_ctx* ctx)
{
    
}

void screen::PlayingMusicScreen::execute(screen_ctx* ctx)
{
    pen::instance()
    .set_position(0, 0)
    .draw_string(song_name_list[ctx->current_music_index])
    .set_position(30, 28);
    if(ctx->current_playing_state == menu::MusicPlayingState::PLAYING)
    {
        pen::instance()
        .draw_string("Playing...");
    }
    else if(ctx->current_playing_state == menu::MusicPlayingState::STOP)
    {
        pen::instance()
        .draw_string("S.T.O.P");
    }

    pen::instance()
    .set_position(10, 40)
    .draw_string("MODE : ");
    if(ctx->_playing_mode == menu::MusicPlayingMode::CIRCLE)
    {
        pen::instance().draw_string("CIRCLE");
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::ONCE)
    {
        pen::instance().draw_string("ONCE");
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::SEQUENTIAL)
    {
        pen::instance().draw_string("SEQUENTIAL");
    }
    else if(ctx->_playing_mode == menu::MusicPlayingMode::RANDOM)
    {
        pen::instance().draw_string("RANDOM");
    }
}