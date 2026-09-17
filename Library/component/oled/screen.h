#ifndef SCREEN_H
#define SCREEN_H

#include "fsm.h"
#include "oled.h"
#include "menu.h"
#include "pattern/picture.h"
#include <stdio.h>
#include "buzzer.h"

inline const char* song_name_list[]=
{
    "Before the Story",
    "BIG SHOT",
    "Black Knife",
    "DELTARUNE ED",
    "Festival",
    "FieldOfHopes&Dreams",
    "Flower Man",
    "Hammer of Justice",
    "Cutie MewMew Magic",
    "Petal Dance",
    "Raise up Your Bat",
    "Running Sky",
    "Shop3",
    "Sunset of SevenSuns",
    "The Third Sanctuary",
    "THE WORLD REVOLVING",
    "TV WORLD",
    "who might you be",
    "Paradise, Paradise",
    "Ruder Buster",
    "FlowerCastle",
    "VioletTactics",
    "WalkingHome",
    "EmptyTown",
    "Vs. Lancer",
    "AttackOfKillerQueen",
    "LostGirl",
    "UntilNextTime",
    "DarkSanctuary",
    "EverHigher",
    "CastleFunk",
    "Friends",
    "From Now On",
    


    "Undyne",
    "ASGORE",


    "Resurrections",
    "QuietAndFalling",

    
};

class screen
{
  public:
    struct screen_ctx
    {
        menu::MenuState state;
        menu::MusicPlayingState playing_state;
        menu::MusicPlayingMode playing_mode;
        int8_t index;
        int8_t music_index;
        int volume; //音量，0-100之间
        int rate;   //倍速乘十
        int tick = 0;
    };

    struct MainScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };
    struct MusicScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };
    struct StandbyScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };
    struct PlayingMusicScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };
    struct DebugScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };
    struct SettingsScreen : public state_t<screen_ctx> {
        void enter(screen_ctx* ctx) override;
        void execute(screen_ctx* ctx) override;
        void exit(screen_ctx* ctx) override{};
    };


    screen_ctx _ctx;
    
    //状态机实例
    fsm_t<screen_ctx> screenFsm;
    MainScreen _mainScreen;
    MusicScreen _musicScreen;
    StandbyScreen _standbyScreen;
    PlayingMusicScreen _playingMusicScreen;
    DebugScreen _debugScreen;
    SettingsScreen _settingsScreen;


    void screen_init();
    void screen_run();
    static screen& instance()
    {
        static screen instance;
        return instance;
    }
};





#endif