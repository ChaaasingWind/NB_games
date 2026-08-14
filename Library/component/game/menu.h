#ifndef MENU_H
#define Menu_H

#include "cmsis_os.h"
#include "stdlib.h"
#include "../../Algo/FSM/fsm.h"
#include "../../Blackboard/blackboard.h"
#include "../../data_def.h"






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

    menuctx _ctx;

    //状态机实例
    MainMenu _mainMenu;
    MusicMenu _musicMenu;




    
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
    void update_cmd(uart_cmd new_cmd);
    


};

#endif