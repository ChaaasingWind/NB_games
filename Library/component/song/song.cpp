#include "song.h"
#include "FreeRTOS.h"
#include "task.h"
#include "menu.h"
#include "stdlib.h"




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
    _ctx._i2s_ctx.final_output = 0;
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        _ctx.count[i]=0;
        _ctx.times[i]=0;
        _ctx._buzzer_ctx.volume[i]=0;
        _ctx._buzzer_ctx.if_start[i]=0;
        _ctx._i2s_ctx.phase[i] = 0;
        _ctx._i2s_ctx.output[i] = 0;
    }
}


void music_play::set_song(const song* new_song)
{
    reset_music();
    _ctx.song_finished = false;
    _ctx._i2s_ctx.virtual_is_finished = false;
    _ctx.current_song = new_song;
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        if(_ctx.current_song->song_voice[i]!=nullptr)
        {
            if(_ctx._buzzer_ctx.if_start[i]==0)
            {
                _ctx._buzzer_ctx.output[i].should_start = true;
                _ctx._buzzer_ctx.volume[i]=_ctx.current_song->song_voice[i]->get_original_volume()*INITIAL_DUTY_CYCLE;
                _ctx._buzzer_ctx.if_start[i]=1;
            }
        }
    }
}


void music_play::set_same_song()
{
    reset_music();
    _ctx.song_finished = false;
    _ctx._i2s_ctx.virtual_is_finished = false;
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        if(_ctx.current_song->song_voice[i]!=nullptr)
        {
            if(_ctx._buzzer_ctx.if_start[i]==0)
            {
                _ctx._buzzer_ctx.output[i].should_start = true;
                _ctx._buzzer_ctx.volume[i]=_ctx.current_song->song_voice[i]->get_original_volume()*INITIAL_DUTY_CYCLE;
                _ctx._buzzer_ctx.if_start[i]=1;
            }
            
        }
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
    SongFsm.enter(this);
}

void music_play::song_run()
{
    static menu::PlayingDevice last_playing_device = menu::PlayingDevice::BUZZER;
    //更新命令
    auto ctx = menu::instance().get_ctx();
    if(ctx._device != last_playing_device)
    {
        if(ctx._device == menu::PlayingDevice::BUZZER)
        {
            SongFsm.change_state(&_buzzer_state);
        }
        else if(ctx._device == menu::PlayingDevice::I2S)
        {
            SongFsm.change_state(&_i2s_state);
        }
    }
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
    SongFsm.execute(this);
    
}