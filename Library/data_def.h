#ifndef DATA_DEF_H
#define DATA_DEF_H

#include "stdint.h"





#define MUSIC_MENU_MAX_NUM 19
#define MAIN_MENU_MAX_NUM 2


union uart_cmd
{
    __attribute__((packed)) struct 
    {
        uint8_t w  : 1  ;
        uint8_t a  : 1  ;
        uint8_t s  : 1  ;
        uint8_t d  : 1  ;
        uint8_t z  : 1  ;
        uint8_t x  : 1  ;
        uint8_t c  : 1  ;
    }msg;

    uint8_t buffer;
};

struct uart_connect_pack
{
    uint8_t head;
    uart_cmd msg;
    uint8_t check;
    uint8_t tail;
};



enum MenuState
{
    MAIN,
    MUSIC,
    STANDBY,
    PLAYING_MUSIC,
};

enum MusicPlayingState
{
    IDLE,
    PLAYING,
    STOP,
};

enum blue_tooth_cmd
{
    NONE,
    YES,
    RETURN,
    TOGGLE,
    UP,
    DOWN,
    LEFT,
    RIGHT,
};


struct menuctx
{
    blue_tooth_cmd cmd;

    MenuState current_state;
    int8_t current_index;

    MusicPlayingState current_playing_state;
    int8_t current_music_index;
    bool music_is_looped; 
    
};







#endif