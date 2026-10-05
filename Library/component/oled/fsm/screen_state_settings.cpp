#include "screen.h"

void screen::SettingsScreen::enter(screen* owner)
{
    
}

void screen::SettingsScreen::execute(screen* owner)
{
    char volume_str[5] = {0};
    sprintf(volume_str, "%d%%", owner->_ctx.volume);
    char rate_str[6] = {0};
    sprintf(rate_str, "%d%%", owner->_ctx.rate*10);
    pen::instance()
    .set_position(0, 20+10*owner->_ctx.index)
    .draw_char('*')
    .set_position(0, 0)
    .set_line_height(8)
    .draw_string("SETTINGS", 0, 2)
    .set_position(10, 20)
    .draw_string("Volume:")
    .set_position(70, 20)
    .draw_string(volume_str)
    .set_position(10, 30)
    .draw_string("Rate:")
    .set_position(70, 30)
    .draw_string(rate_str)
    .set_position(10, 40)
    .draw_string("DEVICE: ");
    if(owner->_ctx.playing_device == menu::PlayingDevice::BUZZER)
    {
        pen::instance().set_position(70, 40)
        .draw_string("BUZZER");
    }
    else if(owner->_ctx.playing_device == menu::PlayingDevice::I2S)
    {
        pen::instance().set_position(70, 40)
        .draw_string("PCM-5102");
    }

}