#include "blackboard.h"




void blackboard::write(uart_cmd cmd)
{
    _uart_cmd = cmd;
}
bool blackboard::read_uart_cmd(uart_cmd* cmd)
{
    *cmd = _uart_cmd;
    return true;
}


void blackboard::write(menuctx ctx)
{
    _menu_ctx =ctx;
}
bool blackboard::read_menu_ctx(menuctx* ctx)
{
    *ctx = _menu_ctx;
    return true;
}