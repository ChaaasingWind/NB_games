#include "song.h"
#include "FreeRTOS.h"
#include "task.h"



extern "C" {
void song_task(void *argument)
{
    vTaskDelay(200);
    music_play::instance().song_init();
    while(1)
    {     
        music_play::instance().song_run();
        //两种状态各自使用一套延时系统，所以下放到各自的状态机execute里面了
    }
}

}