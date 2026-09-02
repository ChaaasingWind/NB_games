#ifndef BUZZER_H
#define BUZZER_H

#include "stdint.h"
#include "main.h"
#include "stdlib.h"
#include "cmath"
#include <cmath>

//设定新音符的初始最大占空比
#define INITIAL_DUTY_CYCLE 0.5f


extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

enum tone
{
    C1,Db1,D1,Eb1,E1,F1,Gb1,G1,Ab1,A1,Bb1,B1,
    C2,Db2,D2,Eb2,E2,F2,Gb2,G2,Ab2,A2,Bb2,B2,
    C3,Db3,D3,Eb3,E3,F3,Gb3,G3,Ab3,A3,Bb3,B3,
    C4,Db4,D4,Eb4,E4,F4,Gb4,G4,Ab4,A4,Bb4,B4,
    C5,Db5,D5,Eb5,E5,F5,Gb5,G5,Ab5,A5,Bb5,B5,
    C6,Db6,D6,Eb6,E6,F6,Gb6,G6,Ab6,A6,Bb6,B6,
    C7,Db7,D7,Eb7,E7,F7,Gb7,G7,Ab7,A7,Bb7,B7,
    NONE_TONE, EMPTY
};

class sound
{
    public:
    

    static constexpr uint16_t prescaler_and_period_arr[86][2]=
    {
        {274, 30581},  // C1
        {274, 28860},  // Db1
        {274, 27241},  // D1
        {274, 25713},  // Eb1
        {274, 24272},  // E1
        {274, 22910},  // F1
        {274, 21622},  // Gb1   
        {274, 20408},  // G1
        {274, 19264},  // Ab1
        {274, 18182},  // A1
        {274, 17161}, // Bb1
        {274, 16197}, // B1

        {67,62750},   // C2 
        {109,36406},  // Db2
        {299,12527},  // D2
        {89,39726},   // Eb2
        {337,9902},   // E2
        {64,49214},   // F2
        {443,6711},   // Gb2
        {51,55022},   // G2
        {80,33107},   // Ab2
        {40,62500},   // A2
        {59,39995},   // Bb2
        {131,17002},  // B2

        {383,5489},   // C3
        {61,32529},   // Db3
        {53,35338},   // D3
        {89,19863},   // Eb3
        {211,7908},   // E3
        {27,58331},   // F3
        {193,7702},   // Gb3
        {33,42517},   // G3
        {21,63064},   // Ab3
        {20,62500},   // A3
        {23,51298},   // Bb3
        {131,8501},   // B3

        {337,3119},   // C4
        {24,41339},   // Db4
        {53,17669},   // D4
        {15,58925},   // Eb4
        {41,20348},   // E4
        {18,43747},   // F4
        {16,46454},   // Gb4
        {31,22630},   // G4
        {12,55181},   // Ab4
        {10,62500},   // A4
        {13,45379},   // Bb4
        {15,37121},   // B4

        {239,2199},   // C5
        {37,13407},   // Db5
        {13,36017},   // D5
        {7,63135},    // Eb5
        {9,46349},    // E5
        {9,43747},    // F5
        {6,61938},   // Gb5
        {7,50110},   // G5
        {9,36787},   // Ab5
        {5,62500},   // A5
        {5,58992},   // Bb5
        {5,55681},   // B5

        {5,52556},   // C6
        {4,62008},   // Db6
        {5,46822},   // D6
        {5,44194},   // Eb6
        {4,52142},   // E6
        {4,49216},    // F6
        {3,61938},   // Gb6
        {5,35077},    // G6
        {7,23649},    // Ab6
        {5,31250},   // A6
        {4,36870},   // Bb6
        {3,46410},    // B6

        {274, 478},   // C7
        {274, 451},   // Db7
        {274, 426},   // D7
        {274, 402},   // Eb7
        {274, 379},   // E7
        {274, 358},   // F7
        {274, 338},   // Gb7
        {274, 319},   // G7
        {274, 301},   // Ab7
        {274, 284},   // A7
        {274, 268},   // Bb7
        {274, 253},   // B7
    {65535,65535}, 
    {65535,65535}};
    

    const uint8_t tone;
    const uint16_t last_beat;
    const uint8_t velocity;

    constexpr sound():tone(tone::NONE_TONE), last_beat(0), velocity(0){}
    constexpr sound(uint16_t last_beat):tone(tone::EMPTY), last_beat(last_beat), velocity(0){};
    constexpr sound(int tone, uint16_t last_beat):
            tone(tone),last_beat(last_beat), velocity(127){};
    constexpr sound(int tone, uint16_t last_beat, uint8_t volume):
            tone(tone),last_beat(last_beat), velocity(volume){};
    

    void convert_frequence_to_pwm_param(uint16_t *prescaler, uint16_t *period) const;
    uint16_t get_original_volume() const {return prescaler_and_period_arr[tone][1];}
    //float get_first_duty() const {return powf((velocity/127.0f *INITIAL_DUTY_CYCLE), 2);}
    float get_first_duty() const 
    {

        float norm = velocity/127.0f;
        float exponent = 3.0f - 0.5f * norm;
        return pow(norm, exponent) * INITIAL_DUTY_CYCLE;
    }


};




struct song
{
    
    const sound* song_voice[5];
    int voice_size[5];
    TIM_HandleTypeDef* htimarr[5];

    uint8_t wait_time;

    song(const sound*p1,const sound*p2,const sound*p3,const sound*p4,const sound*p5,int size1,int size2,int size3,int size4,int size5,uint16_t wait_time)
    {
        song_voice[0]=p1;
        song_voice[1]=p2;
        song_voice[2]=p3;
        song_voice[3]=p4;
        song_voice[4]=p5;
        voice_size[0]=size1;
        voice_size[1]=size2;
        voice_size[2]=size3;
        voice_size[3]=size4;
        voice_size[4]=size5;
        this->wait_time= wait_time;
        htimarr[0]= &htim13;
        htimarr[1]= &htim14;
        htimarr[2]= &htim15;
        htimarr[3]= &htim16;
        htimarr[4]= &htim17;
    }
    static float update_and_return_volume(float now_volume ,const int& original_volume);
};



struct buzzer_tim_output
{
    bool update_tim;
    bool should_stop;
    bool should_start;
    uint16_t prescaler;
    uint16_t autoreload;
    uint16_t compare;
};

struct music_play
{
    int count[5]={0};
    float times[5]={0};
    float volume[5]={0};
    uint8_t if_start[5]={0};
    bool song_finished = false;
    const song* current_song;

    buzzer_tim_output output[5];

    void play_music(float velocity);
    void reset_music();
    void set_song(const song* new_song);
    void set_same_song();
    void set_play_time(int time);
    void keep_silent();
    void set_final_volume(float volume);
    void set_output();
    static music_play& instance()
    {
        static music_play instance;
        return instance;
    }
};



#endif//BUZZER_H