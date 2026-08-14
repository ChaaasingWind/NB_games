#include "buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "songs/before_the_story.h"
#include "task.h"
#include "../../Blackboard/blackboard.h"



#include "songs/song_shop3.h"
#include "songs/song_man.h"
#include "songs/whomightyoube.h"
#include "songs/dr_ed.h"
#include "songs/running_sky.h"
#include "songs/petal_dance.h"
#include "songs/sunsetofsevensuns.h"
#include "songs/mewmew.h"
#include "songs/raise_up_your_bat.h"
#include "songs/the_third_sanctuary.h"
#include "songs/field_of_hopes_and_dreams.h"
#include "songs/black_knife.h"
#include "songs/the_world_revolving.h"
#include "songs/BIG_SHOT.h"
#include "songs/TV_WORLD.h"
#include "songs/Festival.h"
#include "songs/before_the_story.h"
#include "songs/flower_man.h"
#include "songs/hammer_of_justice.h"


const song* song_list[]=
{
    &BeforeTheStory,
    &BIG_SHOT,
    &BlackKnife,
    &dr_ed,
    &Festival,
    &FieldOfHopesAndDreams,
    &FlowerMan,
    &HammerOfJustice,
    &CutieMewMewMagic,
    &PetalDance,
    &RaiseUpYourBat,
    &RunningSky,
    &Shop3, 
    &SunsetOfSevenSuns, 
    &TheThirdSanctuary, 
    &THE_WORLD_REVOLVING, 
    &TVWORLD, 
    &WhoMightYouBe, 
    &Man,
};




extern "C" {
void buzzer_task(void *argument)
{
    
    while(1)
    {
        menuctx cmd;
        blackboard::instance().read_menu_ctx(&cmd);
        static int8_t last_music_index = -1;
        if(cmd.current_music_index != last_music_index)
        {
            music_play::instance().reset_music();
            music_play::instance().set_song(song_list[cmd.current_music_index]);
            last_music_index = cmd.current_music_index;
        }
        if(cmd.current_playing_state == MusicPlayingState::PLAYING)
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
        }
        else if(cmd.current_playing_state == MusicPlayingState::STOP ||
                 cmd.current_playing_state == MusicPlayingState::IDLE)
        {
            music_play::instance().keep_silent();
        }





        
        
        
        
        
        static TickType_t xLastWakeTime = xTaskGetTickCount();
        static const TickType_t xHeartBeat = pdMS_TO_TICKS(1);
        vTaskDelayUntil(&xLastWakeTime, xHeartBeat);
    }
}

}