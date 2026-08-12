#ifndef TEST_TONE_H
#define TEST_TONE_H

#include "../buzzer.h"


#define TEST_TONE Bb1
const sound test_tone_voice1 [] =
{
    {TEST_TONE, 500},
    {TEST_TONE, 500},
    {TEST_TONE, 500},
    {TEST_TONE, 500},
    {TEST_TONE, 500},
    {TEST_TONE, 500},
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