#ifndef SCREEN_H
#define SCREEN_H

#include "fsm.h"
#include "oled.h"
#include "menu.h"
#include "pattern/picture.h"
#include <stdio.h>
#include "song.h"
#include "flash_song_lists.h"


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

    struct MainScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };
    struct MusicScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };
    struct StandbyScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };
    struct PlayingMusicScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };
    struct DebugScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };
    struct SettingsScreen : public state_t<screen> {
        void enter(screen* owner) override;
        void execute(screen* owner) override;
        void exit(screen* owner) override{};
    };


    screen_ctx _ctx;
    
    //状态机实例
    fsm_t<screen> screenFsm;
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