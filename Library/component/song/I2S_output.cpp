#include "song.h"
#include "FreeRTOS.h"
#include "stm32h7xx_hal.h"
#include "task.h"
#include "cmsis_os.h"
#include "cmath"
#include "cstring"
#include "fast_sin.h"

//定义采样频率
constexpr float SAMPLE_FREQUENCY  = 48144.0f;
//则每个数据间的时间间隔为（单位ms）：
constexpr float SAMPLE_DT = 1000.0f/SAMPLE_FREQUENCY;

constexpr float PI = 3.1415926535f;
constexpr float MAX_ONE_SOUND_VOLUME = 4000.0f;

//音符包络参数（ADSR）—— 在"纯指数衰减"与"高保持 ADSR"之间取中
constexpr float ENV_ATTACK_MS   = 3.0f;      //起音时长(ms)
constexpr float ENV_DECAY_MS    = 120.0f;    //衰减到保持电平的时长(ms)
constexpr float ENV_SUSTAIN     = 0.40f;     //保持电平(-8dB)
constexpr float ENV_RELEASE_MS  = 30.0f;     //收音时长(ms)
constexpr float ENV_DECAY_COEFF = 0.99920f;  //每采样衰减系数（120ms 内衰减到 -40dB）



void music_play::I2S_Start()
{

    osSemaphoreAttr_t attr = {0};
    attr.name = "i2s_dma_sem";
    _ctx._i2s_ctx.i2s_transmit_ok = osSemaphoreNew(1, 0, &attr);
    HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t*)audio_buffer, 2048);
    
}


void music_play::set_i2s_fill_state(song_ctx::I2S_ctx::fill_type type)
{
    _ctx._i2s_ctx._type = type;
}

float music_play::compute_current_output(float phase, float time, int last_beat, 
                                        int velocity, int voice_type, int ch)
{
    // 1.包络线：ADSR（每声道独立状态）
    //    起音：线性 0→1
    //    衰减：一阶指数 1→sustain（1 乘 + 1 加）
    //    保持：停在 sustain（零成本）
    //    收音：最后 ENV_RELEASE_MS 内线性淡出到 0，保证音符切换点幅度连续
    float note_ms = (float)last_beat * (float)_ctx.current_song->wait_time;
    float envelope;
    if (time < ENV_ATTACK_MS)
    {
        envelope = time / ENV_ATTACK_MS;                                    // 起音
    }
    else if (time < ENV_ATTACK_MS + ENV_DECAY_MS)
    {
        envelope = ENV_SUSTAIN
                 + (_ctx._i2s_ctx.env[ch] - ENV_SUSTAIN) * ENV_DECAY_COEFF; // 衰减
    }
    else
    {
        envelope = ENV_SUSTAIN;                                             // 保持
    }
    if (note_ms > ENV_RELEASE_MS)                                           // 收音
    {
        float rel = (note_ms - time) / ENV_RELEASE_MS;
        if (rel < envelope) envelope = rel;
    }
    if (envelope < 0.0f) envelope = 0.0f;
    _ctx._i2s_ctx.env[ch] = envelope;


    //2.内部波形合成(全部映射到0-1)
    float wave = 0.0f;
    switch (voice_type)
    {
        case 0 :
        {
            //正弦波
            wave = fastmath::fast_sin(phase);
            break;
        }
        case 1 :
        {
            //方波
            wave = (phase >= 0.5f) ? -1 : 1;
            break;
        }
        case 2 :
        {
            //三角波
            wave = (phase <= 0.5f) ? (phase * 4.0f - 1.0f) : (-phase * 4.0f + 3.0f);
            break;
        }
        case 3 :
        {
            //锯齿波
            wave = phase * 2.0f - 1.0f;
            break;
        }
        case 4 :
        {
            //脉冲波
            wave = (phase >= 0.25f) ? -1 : 1;
            break;
        }
        case 5 :
        {
            //叠加三次谐波
            wave = fastmath::fast_sin(phase) * 0.8f + 0.2f * fastmath::fast_sin(3.0f * phase);
            break;
        }
        case 6 :
        {
            //叠加5次谐波
            wave = fastmath::fast_sin(phase) * 0.8f + 0.2f * fastmath::fast_sin(5.0f * phase);
            break;
        }
        case 7 :
        {
            //双正弦
            wave = (fastmath::fast_sin(phase) + fastmath::fast_sin(phase + 0.02f));
            break;
        }
    }

    return envelope * wave * velocity / 127.0f * MAX_ONE_SOUND_VOLUME;

}

void music_play::init_note_envelope(int ch)
{
    // 复位该声道包络（起音段会重新写入；seek 后直接进入衰减段时从 1.0 起算）
    _ctx._i2s_ctx.env[ch] = 1.0f;
}

void music_play::keep_silent_i2s()
{
    if(!_ctx._i2s_ctx.if_reset)
    {
        memset(audio_buffer, 0, sizeof(audio_buffer));
        _ctx._i2s_ctx.if_reset = true;
    }
}

void music_play::set_play_time_i2s(float time)
{
    //这个函数会让这首歌从固定的时间开始播放，time单位为ms
    for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
    {
        _ctx.count[i]=0;
        _ctx.times[i]=0;
        _ctx._i2s_ctx.phase[i] = 0;
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
            _ctx.times[i] = tick;
            //同步当前音符数据，避免 seek 后 internal_sound_data 仍是旧音符（导致音高/时长错误）
            memcpy(&(_ctx._i2s_ctx.internal_sound_data[i]),
                   _ctx.current_song->song_voice[i] + _ctx.count[i], sizeof(sound));
            init_note_envelope(i);
        }
    }
}

void music_play::play_music_i2s()
{
    if (_ctx.current_song == nullptr) 
    {
        return;
    }
    // 获取接下来填充时的数据偏移量
    int data_offset = 0;
    if(_ctx._i2s_ctx._type == song_ctx::I2S_ctx::fill_type::FirstHalf)
    {
        data_offset = 0;
    }
    else if(_ctx._i2s_ctx._type == song_ctx::I2S_ctx::fill_type::LastHalf)
    {
        data_offset = 1024;
    }
    else 
    {
        return;
    }
    //首先判断是否播放完成
    bool if_over = true;
    for(int i = 0; i < 8; i++)
    {
        if(_ctx.current_song->song_voice[i] == nullptr)
        {
            _ctx._i2s_ctx.output[i] = 0;
            continue;
        }
        if(_ctx.current_song->voice_size[i]>=(_ctx.count[i]+1))
        {
            if_over = false;
        }
        else
        {
            _ctx._i2s_ctx.output[i] = 0;   // 已结束声部清零，避免残留直流偏置
        }
    }
    if(!if_over)
    {
        for(int j = 0; j < 512; j++)
        {

            //同步拍判断
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
                for(int k = 0 ; k < BUZZER_CHANNEL_NUM ; k++)
                {
                    if(_ctx.count[k] + 1 <= _ctx.current_song->voice_size[k])
                    {
                        _ctx.count[k]++;
                        _ctx.times[k]=0;
                        _ctx._i2s_ctx.phase[k] = 0.0f;
                        memcpy(&(_ctx._i2s_ctx.internal_sound_data[k]), _ctx.current_song->song_voice[k] + _ctx.count[k], sizeof(sound));
                        init_note_envelope(k);
                    }

                }
            }
            //遍历
            for(int p = 0; p < BUZZER_CHANNEL_NUM; p++)
            {
                // 1.是否有效
                if(_ctx.current_song->song_voice[p] == nullptr)
                {
                    continue;
                }
            
                // 2.是否结束
                if(_ctx.count[p] + 1 > _ctx.current_song->voice_size[p])
                {
                    continue;
                }

                // 3.计算这个声道此时的音量
                if ((_ctx._i2s_ctx.internal_sound_data[p]).tone == tone::EMPTY ||
                    (_ctx._i2s_ctx.internal_sound_data[p]).tone == tone::NONE_TONE)
                {
                    _ctx._i2s_ctx.output[p] = 0.0f;
                }
                else
                {
                    _ctx._i2s_ctx.output[p] = compute_current_output(
                            _ctx._i2s_ctx.phase[p], _ctx.times[p], 
                            (_ctx._i2s_ctx.internal_sound_data[p]).last_beat,  
                            (_ctx._i2s_ctx.internal_sound_data[p]).velocity, 
                            _ctx.cmd._style, p);
                    //最后相位前进
                    _ctx._i2s_ctx.phase[p] += 
                        (sound::tone_freq_arr[(_ctx._i2s_ctx.internal_sound_data[p]).tone]
                             * SAMPLE_DT * 0.001f * _ctx.cmd.rate *0.1f);
                    if(_ctx._i2s_ctx.phase[p] >= 1.0f)
                    {
                        _ctx._i2s_ctx.phase[p] -= 1.0f;
                    }
                }
                _ctx.times[p] += (SAMPLE_DT * _ctx.cmd.rate *0.1f);

                // 4.处理同步拍
                if((_ctx._i2s_ctx.internal_sound_data[p]).tone==tone::NONE_TONE)
                {
                    continue;
                }
                else if(_ctx.times[p]>=(_ctx._i2s_ctx.internal_sound_data[p]).last_beat*_ctx.current_song->wait_time)
                {
                    //正常切换，开始下一个音符的播放
                    _ctx.count[p]++;
                    _ctx.times[p]=0;
                    _ctx._i2s_ctx.phase[p] = 0.0f;
                    memcpy(&(_ctx._i2s_ctx.internal_sound_data[p]), _ctx.current_song->song_voice[p] + _ctx.count[p], sizeof(sound));
                    init_note_envelope(p);
                    
                }
                
            }
            _ctx.current_time += SAMPLE_DT * _ctx.cmd.rate *0.1f;
            //把最后的输出加和
            _ctx._i2s_ctx.final_output = 0;
            for(int i = 0; i < BUZZER_CHANNEL_NUM; i++)
            {
                _ctx._i2s_ctx.final_output += _ctx._i2s_ctx.output[i];
            }
            _ctx._i2s_ctx.final_output *= (_ctx.cmd.volume * 0.01f);
            audio_buffer[2*j + data_offset] = _ctx._i2s_ctx.final_output;
            audio_buffer[2*j + 1 + data_offset] = _ctx._i2s_ctx.final_output;
        }
    }
    else 
    {
        _ctx._i2s_ctx.virtual_is_finished = true;
    }
}



extern "C"
{


void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{
    if(hi2s == &hi2s2)
    {
        music_play::instance().set_i2s_fill_state(music_play::song_ctx::I2S_ctx::fill_type::FirstHalf);
        if(music_play::instance()._ctx._i2s_ctx.virtual_is_finished)
        {
            music_play::instance()._ctx.song_finished = true;
            music_play::instance()._ctx._i2s_ctx.virtual_is_finished = false;
            music_play::instance().reset_music();
        }
        osSemaphoreRelease(music_play::instance()._ctx._i2s_ctx.i2s_transmit_ok);
    }
}



void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) 
{
    if(hi2s == &hi2s2)
    {
        music_play::instance().set_i2s_fill_state(music_play::song_ctx::I2S_ctx::fill_type::LastHalf);
        if(music_play::instance()._ctx._i2s_ctx.virtual_is_finished)
        {
            music_play::instance()._ctx.song_finished = true;
            music_play::instance()._ctx._i2s_ctx.virtual_is_finished = false;
            music_play::instance().reset_music();
        }
        osSemaphoreRelease(music_play::instance()._ctx._i2s_ctx.i2s_transmit_ok);
    }
}


void HAL_I2S_ErrorCallback(I2S_HandleTypeDef *hi2s) 
{
    if(hi2s == &hi2s2)
    {
        osSemaphoreRelease(music_play::instance()._ctx._i2s_ctx.i2s_transmit_ok);
    }
}

}

// 思路整理
// 目标：得到接下来的缓冲区中的这些数据应该是什么
// 数据实际是由若干声部组合而来
// 我只需要先得到一个声部的音符数据，其它的如法炮制就能实现
// 上一级需求：我怎么得到一个声部的完整的声音数据。
// 定义：1ms内采集数据数为n，那么我需要一个变量，让它在传输数据传输到某一ms中间截断时保留其输出的位置
// 实际上和我之前用到的phase step相似，不过如果后期需要制定各种音色时二者需要分开
// 实际上，每次传输时我只需要根据当前在某一毫秒的位置，继续上一轮的输出，这一毫秒输出的数达到n时，毫秒数就清0，
// 但是phase step不能清零，来保证相位连续。
// 这样子的话，它累计到了1ms的位置，计时器也能正确累加
// 新思路：以采样数作为基本记时单位
// 在切换到新音符时，计算当前音符播放需要多少个采样数，此后进行累加，达到采样数后，就切换到下一个音符
// 但是这个方案有一些难点：1.计时比较麻烦，需要每次获取时转换一下 2.相应的，在播放途中进行蜂鸣器和音频模块的切换比较麻烦
// 最终决策：使用dt+毫秒来进行计时。





//TODO:
//reset函数兼容
//setsong,setsamesong兼容