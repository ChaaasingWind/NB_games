#include "buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "songs/before_the_story.h"
#include "task.h"



// #include "songs/song_shop3.h"
// #include "songs/song_man.h"
// #include "songs/whomightyoube.h"
// #include "songs/dr_ed.h"
// #include "songs/running_sky.h"
// #include "songs/petal_dance.h"
// #include "songs/sunsetofsevensuns.h"
// #include "songs/mewmew.h"
// #include "songs/raise_up_your_bat.h"
// #include "songs/the_third_sanctuary.h"
// #include "songs/field_of_hopes_and_dreams.h"
// #include "songs/black_knife.h"
// #include "songs/the_world_revolving.h"
// #include "songs/BIG_SHOT.h"
// #include "songs/TV_WORLD.h"
#include "songs/Festival.h"
// #include "songs/before_the_story.h"
// #include "songs/flower_man.h"
#include "songs/hammer_of_justice.h"






extern "C" {
void buzzer_task(void *argument)
{
    music_play::instance().set_song(&Festival);
    //music_play::instance().set_play_time(160000);
    while(1)
    {

        if(!music_play::instance().song_finished)
        {
            music_play::instance().play_music();
        }
        else 
        {
            if(music_play::instance().loop_enabled)
            {
                music_play::instance().set_song(music_play::instance().current_song);
            }
        }
        
        
        
        
        static TickType_t xLastWakeTime = xTaskGetTickCount();
        static const TickType_t xHeartBeat = pdMS_TO_TICKS(1);
        vTaskDelayUntil(&xLastWakeTime, xHeartBeat);
    }
}

}