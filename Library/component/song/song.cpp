#include "song.h"
#include <cmath>
#include <cstdint>
#include "FreeRTOS.h"
#include "stm32h7xx_hal_i2s.h"
#include "stm32h7xx_hal_tim.h"
#include "task.h"
#include "menu.h"




int song::get_overall_time()
{
    int time[BUZZER_CHANNEL_NUM] = {0};
    for(int i = 0; i < BUZZER_CHANNEL_NUM ; i++)
    {
        for(int j = 0; j < voice_size[i]; j++)
        {
            //把每个声部的总时间分别求出来；
            time[i] += (song_voice[i]+j)->last_beat;
        }
    }
    int tiiiime = 0;
    tiiiime = time[0] > tiiiime ? time[0]:tiiiime;
    tiiiime = time[1] > tiiiime ? time[1]:tiiiime;
    tiiiime = time[2] > tiiiime ? time[2]:tiiiime;
    tiiiime = time[3] > tiiiime ? time[3]:tiiiime;
    tiiiime = time[4] > tiiiime ? time[4]:tiiiime;
    tiiiime = time[5] > tiiiime ? time[5]:tiiiime;
    tiiiime = time[6] > tiiiime ? time[6]:tiiiime;
    tiiiime = time[7] > tiiiime ? time[7]:tiiiime;
    return tiiiime;
}




void music_play::reset_music()
{
    _ctx.current_time = 0;
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        _ctx.count[i]=0;
        _ctx.times[i]=0;
        _ctx._buzzer_ctx.volume[i]=0;
        _ctx._buzzer_ctx.if_start[i]=0;
    }
}




music_play& music_play::instance()
{
    static music_play instance;
    return instance;
}


int music_play::get_current_song_overall_time()
{
    return _ctx.current_song->overall_time;
}


int music_play::get_current_song_current_time()
{
    return _ctx.current_time;
}


void music_play::song_init()
{
    I2S_Start();
    SongFsm.change_state(&_i2s_state);
    SongFsm.enter(&_ctx);
}

void music_play::song_run()
{

    //更新命令
    auto ctx = menu::instance().get_ctx();
    _ctx.cmd.current_music_index = ctx.current_music_index;
    if(ctx.current_playing_state == menu::MusicPlayingState::PLAYING)
    {
        _ctx.cmd.current_playing_state = song_ctx::song_cmd::playing_state::PLAYING;
    }
    else if(ctx.current_playing_state == menu::MusicPlayingState::STOP)
    {
        _ctx.cmd.current_playing_state = song_ctx::song_cmd::playing_state::STOP;
    }
    else
    {
        _ctx.cmd.current_playing_state = song_ctx::song_cmd::playing_state::IDLE;
    }
    _ctx.cmd.rate = ctx.rate;
    _ctx.cmd.volume = ctx.volume;

    //状态机执行
    SongFsm.execute(&_ctx);
    
}