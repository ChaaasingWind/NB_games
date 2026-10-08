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
    pen::instance()
    .set_position(10, 50)
    .draw_string("Style")
    .set_position(70, 50);
    if(owner->_ctx._style == menu::MusicStyle::CHORUS)
    {
        pen::instance().draw_string("CHORUS");
    }
    else if(owner->_ctx._style == menu::MusicStyle::CLARINET)
    {
        pen::instance().draw_string("CLARINET");
    }
    else if(owner->_ctx._style == menu::MusicStyle::ORGAN)
    {
        pen::instance().draw_string("ORGAN");
    }
    else if(owner->_ctx._style == menu::MusicStyle::ORIGINAL)
    {
        pen::instance().draw_string("ORIGINAL");
    }
    else if(owner->_ctx._style == menu::MusicStyle::PULSE_25)
    {
        pen::instance().draw_string("PULSE_25");
    }
    else if(owner->_ctx._style == menu::MusicStyle::SIN)
    {
        pen::instance().draw_string("SIN");
    }
    else if(owner->_ctx._style == menu::MusicStyle::SQUARE)
    {
        pen::instance().draw_string("SQUARE");
    }
    else if(owner->_ctx._style == menu::MusicStyle::TRINGLE)
    {
        pen::instance().draw_string("TRINGLE");
    }
    else if(owner->_ctx._style == menu::MusicStyle::SAWTOOTH)
    {
        pen::instance().draw_string("SAWTOOTH");
    }

}