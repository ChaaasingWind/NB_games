#include "buzzer.h"
#include <cmath>
#include <cstdint>
#include "FreeRTOS.h"
#include "stm32h7xx_hal_tim.h"
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


// float song::update_and_return_volume(float now_volume , const int& original_volume) 
// {
//     if(now_volume>=0.2*original_volume)
//     {
//         now_volume*=0.9686f;
//     }
//     else if(now_volume<=0.001*original_volume)
//     {
//         now_volume=0;
//     }
//     else 
//     {
//         now_volume*=0.98892f;
//     }
    
    
//     return now_volume;
// }
float song::update_and_return_volume(float now_volume , const int& original_volume) 
{
    if(now_volume>=0.001*original_volume)
    {
        now_volume*=0.99f;
    }
    else 
    {
        now_volume=0;
    }
    
    
    return now_volume;

    
}


void music_play::play_music()
{
    
    if(current_song->voice_size[0]>=(count[0]+1)||
       current_song->voice_size[1]>=(count[1]+1)||
       current_song->voice_size[2]>=(count[2]+1)||
       current_song->voice_size[3]>=(count[3]+1)||
       current_song->voice_size[4]>=(count[4]+1))
    {
        for(int p=0;p<5;p++)
        {
            if(current_song->song_voice[p]==nullptr)
            {
                continue;
            }
            if(count[p]+1>current_song->voice_size[p])
            {
                if(if_start[p])
                {
                    HAL_TIM_PWM_Stop(current_song->htimarr[p], TIM_CHANNEL_1);
                }
                if_start[p] = 0;
                continue;
            }  
            volume[p] = song::update_and_return_volume(volume[p],(current_song->song_voice[p]+count[p])->get_original_volume());
            uint16_t prescaler;
            uint16_t period;
            (current_song->song_voice[p]+count[p])->convert_frequence_to_pwm_param(&prescaler, &period);
            __HAL_TIM_SET_PRESCALER((current_song->htimarr[p]), prescaler-1);
            __HAL_TIM_SetAutoreload((current_song->htimarr[p]), period-1);
            if((current_song->song_voice[p]+count[p])->tone == tone::EMPTY || (current_song->song_voice[p]+count[p])->tone == tone::NONE_TONE)
            {
                __HAL_TIM_SET_COMPARE((current_song->htimarr[p]), TIM_CHANNEL_1, 0);
            }
            else
            {
                __HAL_TIM_SET_COMPARE((current_song->htimarr[p]), TIM_CHANNEL_1, (uint16_t)volume[p]);
            }
            if(times[p]==0)
            {
                (current_song->htimarr[p])->Instance->EGR |= TIM_EGR_UG;
            }
            times[p]++;

            //处理同步拍
            if((current_song->song_voice[p]+count[p])->tone==tone::NONE_TONE)
            {
                if(((current_song->song_voice[0]+count[0])->tone==tone::NONE_TONE||count[0]+1>current_song->voice_size[0])&&
                   ((current_song->song_voice[1]+count[1])->tone==tone::NONE_TONE||count[1]+1>current_song->voice_size[1])&&
                   ((current_song->song_voice[2]+count[2])->tone==tone::NONE_TONE||count[2]+1>current_song->voice_size[2])&&
                   ((current_song->song_voice[3]+count[3])->tone==tone::NONE_TONE||count[3]+1>current_song->voice_size[3])&&
                   ((current_song->song_voice[4]+count[4])->tone==tone::NONE_TONE||count[4]+1>current_song->voice_size[4]))
                {
                    for(int i = 0 ; i < 5 ; i++)
                    {
                        if(count[i]+1<=current_song->voice_size[i])
                        {
                            count[i]++;
                            times[i]=0;
                            volume[i] = (current_song->song_voice[i]+count[i])->get_original_volume()*(current_song->song_voice[i]+count[i])->get_first_duty();
                        }
                        
                    }
                }
                else
                {
                    continue;
                }
            }
            else if(times[p]>=(current_song->song_voice[p]+count[p])->last_beat*current_song->wait_time)
            {
                count[p]++;
                times[p]=0;
                volume[p] = (current_song->song_voice[p]+count[p])->get_original_volume()*(current_song->song_voice[p]+count[p])->get_first_duty();
            }
            
        }
    }
    else
    {
            reset_music();
            song_finished = true;
    }

}

void music_play::reset_music()
{
    for(int i=0;i<5;i++)
    {
        count[i]=0;
        times[i]=0;
        volume[i]=0;
        if_start[i]=0;
    }
}



void music_play::set_song(const song* new_song)
{
    song_finished = false;
    loop_enabled = false;
    current_song = new_song;
    for(int i=0;i<5;i++)
    {
        if(current_song->song_voice[i]!=nullptr)
        {
            if(if_start[i]==0)
            {
                HAL_TIM_PWM_Start((current_song->htimarr[i]), TIM_CHANNEL_1);
                volume[i]=current_song->song_voice[i]->get_original_volume()*INITIAL_DUTY_CYCLE;
                if_start[i]=1;
            }
            
        }
    }
}



void music_play::set_play_time(int time)
{
    //这个函数会让这首歌从固定的时间开始播放，time单位为ms
    for(int i=0;i<5;i++)
    {
        count[i]=0;
        times[i]=0;
        volume[i]=0;
        if_start[i]=0;
        if(current_song->song_voice[i]!=nullptr)
        {
            int tick = time;
            while(tick >= (current_song->song_voice[i]+count[i])->last_beat*current_song->wait_time)
            {
                //先判断指针是否越界
                if(count[i]+1>=current_song->voice_size[i])
                {
                    tick = 0;
                    break;
                }
                tick -= (current_song->song_voice[i]+count[i])->last_beat*current_song->wait_time;
                count[i]++;
            }
            //处理当前音符的播放时间以及音量
            times[i] = tick;
            volume[i] = (current_song->song_voice[i]+count[i])->get_original_volume()*INITIAL_DUTY_CYCLE;
            if(volume[i]>0)
            {
                for(int j=0;j<times[i];j++)
                {
                    volume[i] = song::update_and_return_volume(volume[i],(current_song->song_voice[i]+count[i])->get_original_volume());
                }
            }
            
        }
    }
}


void music_play::keep_silent()
{
    for(int i = 0; i < 5; i++)
    {
        __HAL_TIM_SET_COMPARE((current_song->htimarr[i]), TIM_CHANNEL_1, 0);
    }
}