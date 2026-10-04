#include "song.h"
#include "cmath"
#include <cstdint>

#if FLASH_WRITE_MODE
#include "song_lists.h"
#else
#include "flash_song_lists.h"
#endif




void music_play::I2S_Song::enter(song_ctx* ctx)
{
    music_play::instance().keep_silent();
    music_play::instance().set_output();
}

void music_play::I2S_Song::execute(song_ctx* ctx)
{
    

}

void music_play::I2S_Song::exit(song_ctx* ctx)
{
    

}












//TODO:
//reset函数兼容