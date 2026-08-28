#include "menu.h"
#include "blackboard.h"



void menu::menu_init()
{
    menuFsm.change_state(&_mainMenu);
    menuFsm.enter(&_ctx);
}


void menu::update_cmd(virtual_controller::finalcontroller new_cmd)
{
    static virtual_controller::finalcontroller last_raw_cmd;   // 保存上一次原始按键状态
    
    _ctx.cmd = controller_cmd::NONE;
    
    // 检测按下边沿：当前为1 且 上次为0
    bool z_edge = new_cmd.z == 1 && last_raw_cmd.z == 0;
    bool x_edge = new_cmd.x == 1 && last_raw_cmd.x == 0;
    bool c_edge = new_cmd.c == 1 && last_raw_cmd.c == 0;
    bool w_edge = new_cmd.w == 1 && last_raw_cmd.w == 0;
    bool a_edge = new_cmd.a == 1 && last_raw_cmd.a == 0;
    bool s_edge = new_cmd.s == 1 && last_raw_cmd.s == 0;
    bool d_edge = new_cmd.d == 1 && last_raw_cmd.d == 0;
    
    if(z_edge) _ctx.cmd = controller_cmd::YES;
    else if(x_edge) _ctx.cmd = controller_cmd::TOGGLE;
    else if(c_edge) _ctx.cmd = controller_cmd::RETURN;
    else if(w_edge) _ctx.cmd = controller_cmd::UP;
    else if(a_edge) _ctx.cmd = controller_cmd::LEFT;
    else if(s_edge) _ctx.cmd = controller_cmd::DOWN;
    else if(d_edge) _ctx.cmd = controller_cmd::RIGHT;
    
    last_raw_cmd = new_cmd;   // 更新原始状态
}

void menu::menu_run()
{
    virtual_controller::finalcontroller cmd;
    cmd = virtual_controller::instance().controller;
    update_cmd(cmd);
    menuFsm.execute(&_ctx);
}