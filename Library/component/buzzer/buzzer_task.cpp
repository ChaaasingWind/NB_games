#include "buzzer.h"
#include "main.h"
#include "FreeRTOS.h"
#include "songs/CastleFunk.h"
#include "task.h"
#include "string.h"
#include "menu.h"
#include "song_lists.h"




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
    &RuderBuster,
    &FlowerCastle,
    &VioletTactics,
    &WalkingHome,
    &EmptyTown,
    &Vs_Lancer,
    &AttackOfTheKillerQueen,
    &LostGirl,
    &UntilNextTime,
    &DarkSanctuary,
    &EverHigher,
    &CastleFunk,
    &Friends,
    &From_Now_On,


    &BattleAgainstATrueHero,
    &ASGORE,

    &Resurrections,
    &QuietAndFalling,


    
};



extern "C" {
void buzzer_task(void *argument)
{
    while(1)
    {
        auto ctx = menu::instance().get_ctx();
        static int8_t last_music_index = -1;
        if(ctx.current_music_index != last_music_index)
        {
            music_play::instance().set_song(song_list[ctx.current_music_index]);
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