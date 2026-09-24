#ifndef FLASH_SONG_LISTS
#define FLASH_SONG_LISTS
#include "buzzer.h"
#include "w25q64.h"

constexpr uintptr_t FLASH_MUSIC_BASE = 0x90000000UL;
constexpr int MUSIC_MENU_MAX_NUM = 47;
inline song song_list[MUSIC_MENU_MAX_NUM] = {{}};


#endif