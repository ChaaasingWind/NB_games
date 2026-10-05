#include "song.h"
#include "FreeRTOS.h"
#include "task.h"

#if FLASH_WRITE_MODE

#include "song_lists.h"

#else

#include "flash_song_lists.h"

#endif

void music_play::BuzzerSong::enter(music_play* owner)
{
    owner->set_play_time(owner->get_current_song_current_time());
}

void music_play::BuzzerSong::execute(music_play* owner)
{
    static int8_t last_music_index = -1;
    if(owner->_ctx.cmd.current_music_index != last_music_index)
    {
        owner->set_song(&song_list[owner->_ctx.cmd.current_music_index]);
        last_music_index = owner->_ctx.cmd.current_music_index;
    }
    if(owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::PLAYING)
    {
        if(!owner->_ctx.song_finished)
        {
            owner->play_music();
        }
        else 
        {
            owner->keep_silent();
        }
    }
    else if(owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::STOP ||
             owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::IDLE)
    {
        owner->keep_silent();
    }

    owner->set_final_volume(owner->_ctx.cmd.volume);
    owner->set_output();

    static TickType_t xLastWakeTime = xTaskGetTickCount();
    static const TickType_t xHeartBeat = pdMS_TO_TICKS(1);
    vTaskDelayUntil(&xLastWakeTime, xHeartBeat);
}

void music_play::BuzzerSong::exit(music_play* owner)
{
    owner->keep_silent();
    owner->set_output();
}