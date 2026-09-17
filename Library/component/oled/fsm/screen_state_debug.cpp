#include "screen.h"

extern uint32_t debug;
static char text[50] = {0};


void screen::DebugScreen::enter(screen_ctx* ctx)
{
    
}

void screen::DebugScreen::execute(screen_ctx* ctx)
{

    sprintf(text, "ID : %d", debug);
    pen::instance()
    .set_position(20, 28)
    .draw_string(text);
}