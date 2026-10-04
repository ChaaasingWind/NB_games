#include "song.h"

constexpr float f = 48144.0f;
constexpr float dt = 1.0f/f;
constexpr float PI = 3.1415926535f;



void music_play::I2S_Start()
{
    HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t*)audio_buffer, 1024);
}


void music_play::play_music_i2s()
{


    

}








extern "C"
{


void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s) 
{
    if(hi2s == &hi2s2)
    {

    }
}



void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s) 
{
    if(hi2s == &hi2s2)
    {

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