#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include "../data_def.h"

class blackboard
{
  public:
    //全局单例
    static blackboard& instance()
    {
        static blackboard instance;
        return instance;
    }


    void write(uart_cmd cmd);
    bool read_uart_cmd(uart_cmd* cmd);
    
    void write(menuctx ctx);
    bool read_menu_ctx(menuctx* ctx);


  private:
    uart_cmd _uart_cmd;
    bool _uart_cmd_is_wirting;

    menuctx _menu_ctx;
    bool _menu_ctx_is_wirting;


};








#endif