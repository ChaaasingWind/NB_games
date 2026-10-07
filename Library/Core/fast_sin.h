// fast_sin.h —— 线性插值正弦查表
// 编译期生成正弦表（无需运行时初始化），phase ∈ [0,1) 表示一个完整周期。
#ifndef FAST_SIN_H
#define FAST_SIN_H

#include <cstdint>

namespace fastmath {

constexpr float PI_F     = 3.1415926535f;
constexpr int   SIN_BITS = 10;                 // 表大小 2^10 = 1024 点
constexpr int   SIN_SIZE = 1 << SIN_BITS;      // 1024
constexpr int   SIN_MASK = SIN_SIZE - 1;       // 1023

// 编译期正弦：归约到 [-π/2, π/2] + 泰勒展开到 x^11（误差 ~1e-7），仅用于生成表
constexpr float c_sin(float x)
{
    constexpr float TWO_PI = 6.28318530718f;
    constexpr float PI     = 3.14159265359f;
    if (x > PI)          x -= TWO_PI;          // 输入在 [0, 2π)，归约到 [-π, π]
    if (x >  PI * 0.5f)  x =  PI - x;          // 归约到 [-π/2, π/2]
    if (x < -PI * 0.5f)  x = -PI - x;
    float x2 = x * x;
    return x * (1.0f - x2 * (1.0f / 6.0f - x2 * (1.0f / 120.0f - x2 *
                (1.0f / 5040.0f - x2 * (1.0f / 362880.0f - x2 * (1.0f / 39916800.0f))))));
}

// 表覆盖 [0, 2π)，多存 1 个点（= 首点）方便线性插值取 idx+1
struct SinTable
{
    float v[SIN_SIZE + 1];
    constexpr SinTable() : v{}
    {
        for (int i = 0; i <= SIN_SIZE; ++i)
            v[i] = c_sin(2.0f * PI_F * (float)i / (float)SIN_SIZE);
    }
};

// phase ∈ [0,1) 表示一个完整周期，返回 sin(2π·phase)，范围 [-1, 1]
inline float fast_sin(float phase)
{
    static constexpr SinTable tab{};            // 编译期生成，放 .rodata
    float f    = phase * (float)SIN_SIZE;       // 映射到表坐标
    int   idx  = (int)f;                        // 截断取整
    float frac = f - (float)idx;                // 小数部分，用于插值
    idx &= SIN_MASK;                            // 防止 phase 略越界
    return tab.v[idx] + (tab.v[idx + 1] - tab.v[idx]) * frac;
}

} // namespace fastmath

#endif
