#ifndef TEST_TONE_H
#define TEST_TONE_H

#include "../buzzer.h"



const sound test_tone_voice1 [] =
{
    {C1, 1500},
    {C2, 1500},
    {C3, 1500},
    {C4, 1500},
    {C5, 1500},
    {C6, 1500},
    {C7, 1500},
};


const song TestTone{
    test_tone_voice1,
    nullptr,
    nullptr,
    nullptr,
    nullptr,
    sizeof(test_tone_voice1)/sizeof(sound),
    0,
    0,
    0,
    0,
    1};











#endif