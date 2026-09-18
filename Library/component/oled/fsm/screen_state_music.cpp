#include "screen.h"

void screen::MusicScreen::enter(screen_ctx* ctx)
{
    
}

void screen::MusicScreen::execute(screen_ctx* ctx)
{
    pen::instance()
    .set_position(0, 0)
    .set_line_height(8)
    .draw_string("MUSIC", 0, 2);
    if(ctx->index >= 2)
    {
        pen::instance()
        .set_position(10, 15)
        .draw_string(song_list[ctx->index-2].song_name);
    }
    if(ctx->index >= 1)
    {
        pen::instance()
        .set_position(10, 25)
        .draw_string(song_list[ctx->index-1].song_name);
    }

    pen::instance()
    .set_position(10, 35)
    .draw_string(song_list[ctx->index].song_name)
    .set_position(0, 35)
    .draw_char('*');

    if(ctx->index <=MUSIC_MENU_MAX_NUM-2)
    {
        pen::instance()
        .set_position(10, 45)
        .draw_string(song_list[ctx->index+1].song_name);
    }
    if(ctx->index <=MUSIC_MENU_MAX_NUM-3)
    {
        pen::instance()
        .set_position(10, 55)
        .draw_string(song_list[ctx->index+2].song_name);
    }
}