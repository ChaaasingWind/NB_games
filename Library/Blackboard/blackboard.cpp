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