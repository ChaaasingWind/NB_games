#include "menu.h"



void menu::menu_init()
{
    menuFsm.change_state(&_mainMenu);
    menuFsm.enter(&_ctx);
}


void menu::menu_run()
{
    menuFsm.execute(&_ctx);
}