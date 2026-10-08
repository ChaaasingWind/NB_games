#ifndef SONG_H
#define SONG_H

#include "stdint.h"
#include "main.h"
#include "i2s.h"
#include "stdlib.h"
#include "cmath"
#include <cmath>
#include "fsm.h"
#include "cmsis_os.h"

//设定新音符的初始最大占空比
#define INITIAL_DUTY_CYCLE 0.5f
#define BUZZER_CHANNEL_NUM 8


extern TIM_HandleTypeDef htim13;
extern TIM_HandleTypeDef htim14;
extern TIM_HandleTypeDef htim15;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim12;
extern TIM_HandleTypeDef htim23;

extern I2S_HandleTypeDef hi2s2;

inline __attribute__((section(".sram2"))) int16_t audio_buffer[2048];



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

    // ============================================================
    //  音阶频率表 (Hz) —— 与 enum tone 一一对应
    //  基准：A4 = 440 Hz，十二平均律
    //  C1 = 32.703 Hz ... B7 = 3951.07 Hz
    // ============================================================
    static constexpr float tone_freq_arr[86] = {
        // ---- 八度 1 ----
        32.703f,   // C1
        34.648f,   // Db1
        36.708f,   // D1
        38.891f,   // Eb1
        41.203f,   // E1
        43.654f,   // F1
        46.249f,   // Gb1
        49.000f,   // G1
        51.913f,   // Ab1
        55.000f,   // A1
        58.270f,   // Bb1
        61.735f,   // B1

        // ---- 八度 2 ----
        65.406f,   // C2
        69.296f,   // Db2
        73.416f,   // D2
        77.782f,   // Eb2
        82.407f,   // E2
        87.307f,   // F2
        92.499f,   // Gb2
        97.999f,   // G2
        103.826f,  // Ab2
        110.000f,  // A2
        116.541f,  // Bb2
        123.471f,  // B2

        // ---- 八度 3 ----
        130.813f,  // C3
        138.591f,  // Db3
        146.832f,  // D3
        155.563f,  // Eb3
        164.814f,  // E3
        174.614f,  // F3
        184.997f,  // Gb3
        195.998f,  // G3
        207.652f,  // Ab3
        220.000f,  // A3
        233.082f,  // Bb3
        246.942f,  // B3

        // ---- 八度 4 ----（中央 C 所在八度）
        261.626f,  // C4
        277.183f,  // Db4
        293.665f,  // D4
        311.127f,  // Eb4
        329.628f,  // E4
        349.228f,  // F4
        369.994f,  // Gb4
        391.995f,  // G4
        415.305f,  // Ab4
        440.000f,  // A4 ⭐ 国际标准音
        466.164f,  // Bb4
        493.883f,  // B4

        // ---- 八度 5 ----
        523.251f,  // C5
        554.365f,  // Db5
        587.330f,  // D5
        622.254f,  // Eb5
        659.255f,  // E5
        698.456f,  // F5
        739.989f,  // Gb5
        783.991f,  // G5
        830.609f,  // Ab5
        880.000f,  // A5
        932.328f,  // Bb5
        987.767f,  // B5

        // ---- 八度 6 ----
        1046.502f, // C6
        1108.731f, // Db6
        1174.659f, // D6
        1244.508f, // Eb6
        1318.510f, // E6
        1396.913f, // F6
        1479.978f, // Gb6
        1567.982f, // G6
        1661.219f, // Ab6
        1760.000f, // A6
        1864.655f, // Bb6
        1975.533f, // B6

        // ---- 八度 7 ----
        2093.005f, // C7
        2217.461f, // Db7
        2349.318f, // D7
        2489.016f, // Eb7
        2637.021f, // E7
        2793.826f, // F7
        2959.955f, // Gb7
        3135.964f, // G7
        3322.438f, // Ab7
        3520.000f, // A7
        3729.310f, // Bb7
        3951.066f, // B7

        // ---- 特殊值 ----
        0.0f,      // NONE_TONE —— 同步点，不发声
        0.0f       // EMPTY    —— 空白段，不发声
    };

    const uint8_t tone;
    const uint8_t velocity;
    const uint16_t last_beat;
    

    constexpr sound():tone(tone::NONE_TONE), velocity(0), last_beat(0){}
    constexpr sound(uint16_t last_beat):tone(tone::EMPTY), velocity(0), last_beat(last_beat){};
    constexpr sound(int tone, uint16_t last_beat):
            tone(tone), velocity(127),last_beat(last_beat){};
    constexpr sound(int tone, uint16_t last_beat, uint8_t volume):
            tone(tone), velocity(volume),last_beat(last_beat){};
    

    //buzzer专属
    void convert_frequence_to_pwm_param(uint16_t *prescaler, uint16_t *period) const;
    uint16_t get_original_volume() const;
    float get_first_duty() const;
};




struct song
{
    
    const sound* song_voice[BUZZER_CHANNEL_NUM];
    int voice_size[BUZZER_CHANNEL_NUM];
    TIM_HandleTypeDef* htimarr[BUZZER_CHANNEL_NUM];
    uint8_t wait_time;
    int overall_time;
    const char* song_name;

    //buzzer专属
    static float update_and_return_volume(float now_volume ,const int& original_volume);

    int get_overall_time();
    song(const sound*p1,
         const sound*p2,
         const sound*p3,
         const sound*p4,
         const sound*p5,
         const sound*p6,
         const sound*p7,
         const sound*p8,
         int size1,
         int size2,
         int size3,
         int size4,
         int size5,
         int size6,
         int size7,
         int size8,
         uint16_t wait_time,
         const char* name)
    {
        song_voice[0]=p1;
        song_voice[1]=p2;
        song_voice[2]=p3;
        song_voice[3]=p4;
        song_voice[4]=p5;
        song_voice[5]=p6;
        song_voice[6]=p7;
        song_voice[7]=p8;
        voice_size[0]=size1;
        voice_size[1]=size2;
        voice_size[2]=size3;
        voice_size[3]=size4;
        voice_size[4]=size5;
        voice_size[5]=size6;
        voice_size[6]=size7;
        voice_size[7]=size8;
        this->wait_time= wait_time;
        this->song_name = name;
        htimarr[0]= &htim13;
        htimarr[1]= &htim14;
        htimarr[2]= &htim15;
        htimarr[3]= &htim16;
        htimarr[4]= &htim17;
        htimarr[5]= &htim2;
        htimarr[6]= &htim12;
        htimarr[7]= &htim23;
        overall_time = get_overall_time();
    }
    
    song() 
    {
        for (int i = 0; i < BUZZER_CHANNEL_NUM; i++) 
        {
            song_voice[i] = nullptr;
            voice_size[i] = 0;
        }
        htimarr[0]= &htim13;
        htimarr[1]= &htim14;
        htimarr[2]= &htim15;
        htimarr[3]= &htim16;
        htimarr[4]= &htim17;
        htimarr[5]= &htim2;
        htimarr[6]= &htim12;
        htimarr[7]= &htim23;
        this->song_name = nullptr;
        wait_time = 1;
        overall_time = 0;
    }
};





struct music_play
{
    struct song_ctx
    {
        struct song_cmd
        {
            enum class playing_state
            {
                IDLE,
                PLAYING,
                STOP,
            };
            int current_music_index;
            playing_state current_playing_state;
            int rate;
            int volume;
            int _style;
        };
        struct buzzer_ctx
        {
            struct buzzer_tim_output
            {
                bool update_tim;
                bool should_stop;
                bool should_start;
                uint16_t prescaler;
                uint16_t autoreload;
                uint16_t compare;
            };
            float volume[BUZZER_CHANNEL_NUM]={0};
            buzzer_tim_output output[BUZZER_CHANNEL_NUM];
            uint8_t if_start[BUZZER_CHANNEL_NUM]={0};   //当前声道是否开启
        };
        struct I2S_ctx
        {
            enum class fill_type
            {
                NONE,
                FirstHalf,
                LastHalf,
            };
            sound internal_sound_data[BUZZER_CHANNEL_NUM];
            bool if_reset = false;
            osSemaphoreId_t i2s_transmit_ok;
            fill_type _type = fill_type::NONE;
            bool virtual_is_finished = false;
            float phase[BUZZER_CHANNEL_NUM]  = {0};     //1ms内的时间
            float output[BUZZER_CHANNEL_NUM] = {0};
            float env[BUZZER_CHANNEL_NUM] = {0};        //每声道包络当前值
            float final_output = 0;
        };
        I2S_ctx _i2s_ctx;
        buzzer_ctx _buzzer_ctx;
        song_cmd cmd;

        int count[BUZZER_CHANNEL_NUM]={0};          //音符事件数
        float times[BUZZER_CHANNEL_NUM]={0};        //当前音符播放的毫秒数
        bool song_finished = false;
        float current_time = 0;
        const song* current_song = nullptr;
        int8_t last_music_index = -1;               //当前已加载的歌曲索引（I2S/蜂鸣器两个状态共享，避免切换设备时误触发 set_song 而重置进度）

    };
    song_ctx _ctx;
    
    
    //两种输出下共享的函数
    static music_play& instance();
    int get_current_song_overall_time();
    int get_current_song_current_time();
    void reset_music();
    void song_init();
    void song_run();
    struct BuzzerSong : public state_t<music_play> 
    {
        void enter(music_play* owner) override;
        void execute(music_play* owner) override;
        void exit(music_play* owner) override;
    };
    struct I2S_Song : public state_t<music_play> {
        void enter(music_play* owner) override;
        void execute(music_play* owner) override;
        void exit(music_play* owner) override;
    };

    fsm_t<music_play> SongFsm;
    BuzzerSong _buzzer_state;
    I2S_Song _i2s_state;

    //蜂鸣器输出下的音乐播放相关函数
    void play_music();
    void set_song(const song* new_song);
    void set_same_song();
    void set_play_time(float time);
    void keep_silent();
    void set_final_volume(float volume);
    void set_output();

    //I2S输出下的音乐播放相关函数
    void I2S_Start();
    void set_i2s_fill_state(song_ctx::I2S_ctx::fill_type type);
    void play_music_i2s();
    void set_play_time_i2s(float time);
    void keep_silent_i2s();
    // 计算单个音调在当前时间下应该输出多少
    float compute_current_output(float phase, float time, int last_beat, int velocity, int voice_type, int ch);
    // 音符开始时初始化该声道的包络（衰减系数按音符时长计算）
    void init_note_envelope(int ch);
    
    
};



#endif