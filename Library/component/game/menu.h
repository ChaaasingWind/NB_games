#ifndef MENU_H
#define MENU_H

#include "cmsis_os.h"
#include "stdlib.h"
#include "fsm.h"
#include "vrc.h"


constexpr int MUSIC_MENU_MAX_NUM = 25;
constexpr int MAIN_MENU_MAX_NUM  = 4;





class menu
{
  public:

      enum class MenuState
    {
        MAIN,
        MUSIC,
        STANDBY,
        PLAYING_MUSIC,
        SETTINGS,
        _DEBUG,
    };

    enum class MusicPlayingState
    {
        IDLE,
        PLAYING,
        STOP,
    };

    struct menuctx
    {
        MenuState current_state;
        int8_t current_index;

        MusicPlayingState current_playing_state;
        int8_t current_music_index;
        bool music_is_looped; 
        int volume =100;
    };



    struct MainMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };
    struct MusicMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };
    struct StandbyMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };
    struct PlayingMusicMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };
    struct DebugMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };
    struct SettingsMenu : public state_t<menuctx> {
        void enter(menuctx* ctx) override;
        void execute(menuctx* ctx) override;
        void exit(menuctx* ctx) override{};
    };

    menuctx _ctx;

    //状态机实例
    fsm_t<menuctx> menuFsm;
    MainMenu _mainMenu;
    MusicMenu _musicMenu;
    StandbyMenu _standbyMenu;
    PlayingMusicMenu _playingMusicMenu;
    DebugMenu _debug_Menu;
    SettingsMenu _settingsMenu;

    state_t<menuctx>* MainMenuOptions[4] = {&_musicMenu, &_standbyMenu, &_settingsMenu, &_debug_Menu};


    
    static menu& instance()
    {
        static menu instance;
        return instance;
    }
    menuctx read_ctx()
    {
        return _ctx;
    }
    void menu_init();
    void menu_run();
    menuctx get_ctx()
    {
        return _ctx;
    }
    void update_cmd(virtual_controller::finalcontroller new_cmd);
    


};

#endif