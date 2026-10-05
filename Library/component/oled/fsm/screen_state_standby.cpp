#include "screen.h"

void screen::StandbyScreen::enter(screen* owner)
{
    
}

void screen::StandbyScreen::execute(screen* owner)
{
    pen::instance()
    .set_position(0, 0)
    .draw_gif(gif_Pink_face_shop_animation_Orange, owner->_ctx.tick, 30);
}