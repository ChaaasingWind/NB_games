#include "screen.h"



void screen::screen_init()
{
    pen::instance()
    .set_position(0,0)
    .set_mode(0)
    .set_line_height(8);
    screenFsm.change_state(&_mainScreen);
    screenFsm.enter(&_ctx);
}


void screen::screen_run()
{
    //清屏
    memset(oled_arr, 0, 1024);

    //更新指令
    auto ctx = menu::instance().get_ctx();
    _ctx._playing_mode            = ctx._playing_mode;
    _ctx.current_playing_state    = ctx.current_playing_state;
    _ctx.current_music_index      = ctx.current_music_index;
    _ctx.current_index            = ctx.current_index;
    _ctx._playing_mode            = ctx._playing_mode;
    _ctx.current_state            = ctx.current_state;
    _ctx.rate                     = ctx.rate;
    _ctx.volume                   = ctx.volume;
    _ctx.tick++;

    //状态机运行
    if(_ctx.current_state == menu::MenuState::MAIN)
    {
        screenFsm.change_state(&_mainScreen);
    }
    else if(_ctx.current_state == menu::MenuState::MUSIC)
    {
        screenFsm.change_state(&_musicScreen);
    }
    else if(_ctx.current_state == menu::MenuState::STANDBY)
    {
        screenFsm.change_state(&_standbyScreen);
    }
    else if(_ctx.current_state == menu::MenuState::PLAYING_MUSIC)
    {
        screenFsm.change_state(&_playingMusicScreen);
    }
    else if(_ctx.current_state == menu::MenuState::SETTINGS)
    {
        screenFsm.change_state(&_settingsScreen);
    }
    else if(_ctx.current_state == menu::MenuState::_DEBUG)
    {
        screenFsm.change_state(&_debugScreen);
    }

    screenFsm.execute(&_ctx);


    //写到buffer中
    memcpy(oled_buffer, oled_arr, 1024); 
    osSemaphoreRelease(send_sem);
}