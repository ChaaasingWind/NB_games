#include "screen.h"

void screen::StandbyScreen::enter(screen_ctx* ctx)
{
    
}

void screen::StandbyScreen::execute(screen_ctx* ctx)
{
    pen::instance()
    .set_position(0, 0)
    .draw_gif(gif_Pink_face_shop_animation_Orange, ctx->tick, 30);
}