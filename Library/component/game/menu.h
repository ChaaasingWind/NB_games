#ifndef MENU_H
#define MENU_H

#include "cmsis_os.h"
#include "stdlib.h"
#include "fsm.h"
#include "data_def.h"
#include "vrc.h"






static fsm_t<menuctx> menuFsm;

class menu
{
  public:

  
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

    menuctx _ctx;

    //状态机实例
    MainMenu _mainMenu;
    MusicMenu _musicMenu;
    StandbyMenu _standbyMenu;
    PlayingMusicMenu _playingMusicMenu;
    DebugMenu _debug_Menu;

    state_t<menuctx>* MainMenuOptions[3] = {&_musicMenu, &_standbyMenu, &_debug_Menu};


    
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
    void update_cmd(virtual_controller::finalcontroller new_cmd);
    


};

#endif