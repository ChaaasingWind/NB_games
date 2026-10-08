#include "song.h"
#include "FreeRTOS.h"
#include "task.h"



void sound::convert_frequence_to_pwm_param(uint16_t *prescaler, uint16_t *period) const
{
    if(last_beat!=0)
    {
        *prescaler=prescaler_and_period_arr[tone][0];
        *period=prescaler_and_period_arr[tone][1];
    }
    else 
    {
        *prescaler=65535;
        *period=65535;
    }
    
}


uint16_t sound::get_original_volume() const 
{
    return prescaler_and_period_arr[tone][1];
}


float sound::get_first_duty() const 
{
    float norm = velocity/127.0f;
    float exponent = 3.0f - 0.5f * norm;
    return pow(norm, exponent) * INITIAL_DUTY_CYCLE;
}

float song::update_and_return_volume(float now_volume , const int& original_volume) 
{
    if(now_volume>=0.0001*original_volume)
    {
        now_volume*=0.99f;
    }
    
    return now_volume;
}






void music_play::set_play_time(float time)
{
    //这个函数会让这首歌从固定的时间开始播放，time单位为ms
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        _ctx.count[i]=0;
        _ctx.times[i]=0;
        _ctx._buzzer_ctx.volume[i]=0;
        _ctx._buzzer_ctx.if_start[i]=1;
        if(_ctx.current_song->song_voice[i]!=nullptr)
        {
            float tick = time;
            while(tick >= (_ctx.current_song->song_voice[i]+_ctx.count[i])->last_beat*_ctx.current_song->wait_time)
            {
                //先判断指针是否越界
                if(_ctx.count[i]+1>=_ctx.current_song->voice_size[i])
                {
                    tick = 0;
                    break;
                }
                tick -= (_ctx.current_song->song_voice[i]+_ctx.count[i])->last_beat*_ctx.current_song->wait_time;
                _ctx.count[i]++;
            }
            //处理当前音符的播放时间以及音量
            _ctx.times[i] = tick;
            _ctx._buzzer_ctx.volume[i] = (_ctx.current_song->song_voice[i]+_ctx.count[i])->get_original_volume()*INITIAL_DUTY_CYCLE;
            if(_ctx._buzzer_ctx.volume[i]>0)
            {
                for(int j=0;j<_ctx.times[i];j++)
                {
                    _ctx._buzzer_ctx.volume[i] = song::update_and_return_volume(_ctx._buzzer_ctx.volume[i],(_ctx.current_song->song_voice[i]+_ctx.count[i])->get_original_volume());
                }
            }
            
        }
    }
}


void music_play::keep_silent()
{
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        _ctx._buzzer_ctx.output[i].compare = 0;
        _ctx._buzzer_ctx.output[i].update_tim = 1;
    }
}




void music_play::set_output()
{
    if(_ctx.current_song == nullptr)
    {
        return;
    }
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        if(_ctx._buzzer_ctx.output[i].should_stop)
        {
            HAL_TIM_PWM_Stop((_ctx.current_song->htimarr[i]), TIM_CHANNEL_1);
            _ctx._buzzer_ctx.output[i].should_stop = false;
        }
        if(_ctx._buzzer_ctx.output[i].should_start)
        {
            HAL_TIM_PWM_Start((_ctx.current_song->htimarr[i]), TIM_CHANNEL_1);
            _ctx._buzzer_ctx.output[i].should_start = false;
        }


        __HAL_TIM_SetAutoreload((_ctx.current_song->htimarr[i]), _ctx._buzzer_ctx.output[i].autoreload-1);
        __HAL_TIM_SET_PRESCALER((_ctx.current_song->htimarr[i]), _ctx._buzzer_ctx.output[i].prescaler-1);
        __HAL_TIM_SET_COMPARE((_ctx.current_song->htimarr[i]), TIM_CHANNEL_1, _ctx._buzzer_ctx.output[i].compare);

        if(_ctx._buzzer_ctx.output[i].update_tim)
        {
            (_ctx.current_song->htimarr[i])->Instance->EGR |= TIM_EGR_UG;
            _ctx._buzzer_ctx.output[i].update_tim = false;
        }
    }
}



void music_play::set_final_volume(float volume)
{
    for(int i = 0; i< BUZZER_CHANNEL_NUM; i++)
    {
        _ctx._buzzer_ctx.output[i].compare *= (volume/100.0f);
    }
}




void music_play::play_music()
{
    //首先判断是否播放完成
    bool if_over = true;
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        if(_ctx.current_song->song_voice[i] == nullptr)
        {
            continue;
        }
        if(_ctx.current_song->voice_size[i]>=(_ctx.count[i]+1))
        {
            if_over = false;
        }
    }
    if(!if_over)
    {
        for(int p=0;p<BUZZER_CHANNEL_NUM;p++)
        {
            if(_ctx.current_song->song_voice[p]==nullptr)
            {
                continue;
            }
            if(_ctx.count[p] + 1 > _ctx.current_song->voice_size[p])
            {
                if(_ctx._buzzer_ctx.if_start[p])
                {
                    HAL_TIM_PWM_Stop(_ctx.current_song->htimarr[p], TIM_CHANNEL_1);
                }
                _ctx._buzzer_ctx.if_start[p] = 0;
                continue;
            }  
            _ctx._buzzer_ctx.volume[p] = song::update_and_return_volume(_ctx._buzzer_ctx.volume[p],
                        (_ctx.current_song->song_voice[p]+_ctx.count[p])->get_original_volume());
            uint16_t prescaler;
            uint16_t period;
            (_ctx.current_song->song_voice[p]+_ctx.count[p])->convert_frequence_to_pwm_param(
                        &prescaler, &period);
            _ctx._buzzer_ctx.output[p].prescaler = prescaler;
            _ctx._buzzer_ctx.output[p].autoreload = period;

            if((_ctx.current_song->song_voice[p]+_ctx.count[p])->tone == tone::EMPTY ||
                (_ctx.current_song->song_voice[p]+_ctx.count[p])->tone == tone::NONE_TONE)
            {
                _ctx._buzzer_ctx.output[p].compare = 0;
            }
            else
            {
                _ctx._buzzer_ctx.output[p].compare = (uint16_t)_ctx._buzzer_ctx.volume[p];
            }
            if(_ctx.times[p]==0)
            {
                _ctx._buzzer_ctx.output[p].update_tim = true;
            }
            _ctx.times[p] += _ctx.cmd.rate*0.1f;
            

            //处理同步拍
            if((_ctx.current_song->song_voice[p]+_ctx.count[p])->tone==tone::NONE_TONE)
            {
                bool all_none = true;
                for (int i = 0; i < BUZZER_CHANNEL_NUM; i++) 
                {
                    if (_ctx.current_song->song_voice[i] == nullptr) continue;
                    if (_ctx.count[i] >= _ctx.current_song->voice_size[i]) continue;
                    if ((_ctx.current_song->song_voice[i] + _ctx.count[i])->tone != tone::NONE_TONE) {
                        all_none = false;
                        break;
                    }
                }
                if(all_none)
                {
                    for(int i = 0 ; i < BUZZER_CHANNEL_NUM ; i++)
                    {
                        if(_ctx.count[i]+1<=_ctx.current_song->voice_size[i])
                        {
                            _ctx.count[i]++;
                            _ctx.times[i]=0;
                            _ctx._buzzer_ctx.volume[i] = (_ctx.current_song->song_voice[i]+_ctx.count[i])->
                                get_original_volume()*(_ctx.current_song->song_voice[i]+_ctx.count[i])->get_first_duty();
                        }
                        
                    }
                }
                else
                {
                    continue;
                }
            }
            else if(_ctx.times[p]>=(_ctx.current_song->song_voice[p]+_ctx.count[p])->last_beat*_ctx.current_song->wait_time)
            {
                _ctx.count[p]++;
                _ctx.times[p]=0;
                _ctx._buzzer_ctx.volume[p] = (_ctx.current_song->song_voice[p]+_ctx.count[p])->
                    get_original_volume()*(_ctx.current_song->song_voice[p]+_ctx.count[p])->get_first_duty();
            }
            
        }
        _ctx.current_time += _ctx.cmd.rate*0.1f;
    }
    else
    {
            reset_music();
            _ctx.song_finished = true;
    }
}