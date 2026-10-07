#include "song.h"
#include "FreeRTOS.h"
#include "task.h"

#if FLASH_WRITE_MODE
#include "song_lists.h"
#else
#include "flash_song_lists.h"
#endif




void music_play::I2S_Song::enter(music_play* owner)
{
    if(owner->_ctx.current_song != nullptr)
    {
        owner->set_play_time_i2s(owner->get_current_song_current_time());
    }
    
}

void music_play::I2S_Song::execute(music_play* owner)
{
    static int8_t last_music_index = -1;
    if(owner->_ctx.cmd.current_music_index != last_music_index)
    {
        owner->set_song(&song_list[owner->_ctx.cmd.current_music_index]);
        last_music_index = owner->_ctx.cmd.current_music_index;
    }
    if(owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::PLAYING)
    {
        if(!owner->_ctx.song_finished)
        {
            owner->_ctx._i2s_ctx.if_reset = false;
            if (osSemaphoreAcquire(owner->_ctx._i2s_ctx.i2s_transmit_ok, pdMS_TO_TICKS(100)) != osOK) 
            {
                // 超时了，可能是 DMA 停了
                return;   // 这次不填，下一轮再试
            }
            owner->play_music_i2s();
        }
        else 
        {
            owner->keep_silent_i2s();
            // 清空所有堆积的信号量（非阻塞）
            osSemaphoreAcquire(owner->_ctx._i2s_ctx.i2s_transmit_ok, 0);
            // 再延时，降低 CPU 占用
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
    else if(owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::STOP ||
             owner->_ctx.cmd.current_playing_state == song_ctx::song_cmd::playing_state::IDLE)
    {
        owner->keep_silent_i2s();
        // 清空所有堆积的信号量（非阻塞）
        osSemaphoreAcquire(owner->_ctx._i2s_ctx.i2s_transmit_ok, 0);
        // 再延时，降低 CPU 占用
        vTaskDelay(pdMS_TO_TICKS(10));
    }
    
}

void music_play::I2S_Song::exit(music_play* owner)
{
    owner->keep_silent_i2s();

}











