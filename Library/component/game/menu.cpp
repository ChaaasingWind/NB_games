#include "menu.h"




void menu::menu_init()
{
    menuFsm.change_state(&_mainMenu);
    menuFsm.enter(&_ctx);
}


void menu::update_cmd(uart_cmd new_cmd)
{
    static uart_cmd last_raw_cmd = {.buffer = 0};   // 保存上一次原始按键状态
    static blue_tooth_cmd last_cmd = blue_tooth_cmd::NONE;
    
    _ctx.cmd = blue_tooth_cmd::NONE;
    
    // 检测按下边沿：当前为1 且 上次为0
    bool z_edge = new_cmd.msg.z == 1 && last_raw_cmd.msg.z == 0;
    bool x_edge = new_cmd.msg.x == 1 && last_raw_cmd.msg.x == 0;
    bool c_edge = new_cmd.msg.c == 1 && last_raw_cmd.msg.c == 0;
    bool w_edge = new_cmd.msg.w == 1 && last_raw_cmd.msg.w == 0;
    bool a_edge = new_cmd.msg.a == 1 && last_raw_cmd.msg.a == 0;
    bool s_edge = new_cmd.msg.s == 1 && last_raw_cmd.msg.s == 0;
    bool d_edge = new_cmd.msg.d == 1 && last_raw_cmd.msg.d == 0;
    
    if(z_edge) _ctx.cmd = blue_tooth_cmd::YES;
    else if(x_edge) _ctx.cmd = blue_tooth_cmd::PAUSE;
    else if(c_edge) _ctx.cmd = blue_tooth_cmd::RETURN;
    else if(w_edge) _ctx.cmd = blue_tooth_cmd::UP;
    else if(a_edge) _ctx.cmd = blue_tooth_cmd::LEFT;
    else if(s_edge) _ctx.cmd = blue_tooth_cmd::DOWN;
    else if(d_edge) _ctx.cmd = blue_tooth_cmd::RIGHT;
    
    last_raw_cmd = new_cmd;   // 更新原始状态
    last_cmd = _ctx.cmd;
}

void menu::menu_run()
{
    uart_cmd uartcmd;
    blackboard::instance().read_uart_cmd(&uartcmd);
    update_cmd(uartcmd);
    menuFsm.execute(&_ctx);
}