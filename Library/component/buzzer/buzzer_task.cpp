#include "buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "songs/Paradise_Paradise.h"
#include "songs/before_the_story.h"
#include "task.h"
#include "blackboard.h"



#include "shop3.h"
#include "whomightyoube.h"
#include "dr_ed.h"
#include "running_sky.h"
#include "petal_dance.h"
#include "sunsetofsevensuns.h"
#include "mewmew.h"
#include "raise_up_your_bat.h"
#include "the_third_sanctuary.h"
#include "field_of_hopes_and_dreams.h"
#include "black_knife.h"
#include "the_world_revolving.h"
#include "BIG_SHOT.h"
#include "TV_WORLD.h"
#include "Festival.h"
#include "before_the_story.h"
#include "flower_man.h"
#include "hammer_of_justice.h"
#include "Paradise_Paradise.h"
#include "i_miss_you_zhang.h"
#include "RuderBuster.h"
#include "BattleAgainstATrueHero.h"
#include "ASGORE.h"

const song* song_list[]=
{
    &BeforeTheStory,
    &BIG_SHOT,
    &BlackKnife,
    &DrEd,
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
    &TV_WORLD, 
    &WhoMightYouBe, 
    &Paradise_Paradise,
    &i_miss_you_zhang,
    &RuderBuster,
    &BattleAgainstATrueHero,
    &ASGORE,
};




extern "C" {
void buzzer_task(void *argument)
{
    // music_play::instance().set_song(&TestTone);
    // music_play::instance().loop_enabled = true;
    while(1)
    {
        menuctx cmd;
        blackboard::instance().read_menu_ctx(&cmd);
        static int8_t last_music_index = -1;
        music_play::instance().loop_enabled = cmd.music_is_looped;
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
                else 
                {
                    music_play::instance().keep_silent();
                }
            }
        }
        else if(cmd.current_playing_state == MusicPlayingState::STOP ||
                 cmd.current_playing_state == MusicPlayingState::IDLE)
        {
            music_play::instance().keep_silent();
        }





        
        // if(!music_play::instance().song_finished)
        // {
        //     music_play::instance().play_music();
        // }
        // else 
        // {
        //     if(music_play::instance().loop_enabled)
        //     {
        //         music_play::instance().set_song(music_play::instance().current_song);
             
        //     }
        //     else 
        //     {
        //         music_play::instance().keep_silent();
        //     }
        // }
        
        
        
        static TickType_t xLastWakeTime = xTaskGetTickCount();
        static const TickType_t xHeartBeat = pdMS_TO_TICKS(1);
        vTaskDelayUntil(&xLastWakeTime, xHeartBeat);
    }
}

}