#include "buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "songs/CastleFunk.h"
#include "task.h"
#include "string.h"
#include "menu.h"

#if FLASH_WRITE_MODE

#include "song_lists.h"

#else

#include "flash_song_lists.h"

#endif






extern "C" {
void buzzer_task(void *argument)
{
    vTaskDelay(100);
    while(1)
    {
        auto ctx = menu::instance().get_ctx();
        static int8_t last_music_index = -1;
        if(ctx.current_music_index != last_music_index)
        {
            music_play::instance().set_song(&song_list[ctx.current_music_index]);
            last_music_index = ctx.current_music_index;
        }
        if(ctx.current_playing_state == menu::MusicPlayingState::PLAYING)
        {
            if(!music_play::instance().song_finished)
            {
                music_play::instance().play_music(ctx.rate*0.1f);
            }
            else 
            {
                music_play::instance().keep_silent();
            }
        }
        else if(ctx.current_playing_state == menu::MusicPlayingState::STOP ||
                 ctx.current_playing_state == menu::MusicPlayingState::IDLE)
        {
            music_play::instance().keep_silent();
        }

        music_play::instance().set_final_volume(ctx.volume);
        music_play::instance().set_output();


        static TickType_t xLastWakeTime = xTaskGetTickCount();
        static const TickType_t xHeartBeat = pdMS_TO_TICKS(1);
        vTaskDelayUntil(&xLastWakeTime, xHeartBeat);
    }
}

}