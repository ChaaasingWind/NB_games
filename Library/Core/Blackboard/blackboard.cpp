#include "blackboard.h"





void blackboard::write(menuctx ctx)
{
    _menu_ctx =ctx;
}
bool blackboard::read_menu_ctx(menuctx* ctx)
{
    *ctx = _menu_ctx;
    return true;
}