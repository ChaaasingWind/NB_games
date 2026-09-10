#include "screen.h"

void screen::MainScreen::enter(screen_ctx* ctx)
{
    
}

void screen::MainScreen::execute(screen_ctx* ctx)
{
    pen::instance()
    .set_position(0, 0)
    .set_line_height(8)
    .draw_string("MAIN", 0, 2)
    .set_position(10, 20)
    .draw_string("MUSIC")
    .set_position(10, 30)
    .draw_string("STANDBY")
    .set_position(10, 40)
    .draw_string("SETTINGS")
    .set_position(10, 50)
    .draw_string("DEBUG")
    .set_position(0, 20+10*ctx->current_index)
    .draw_char('*')
    .set_position(80, 15)
    .draw_pattern(picture_Ralsei_face_battlemenuData);
}