#include "menu.h"




void menu::menu_init()
{
    menuFsm.change_state(&_mainMenu);
    menuFsm.enter(&_ctx);
}


void menu::menu_run()
{
    //检查播放器是否播放完成，如果播放完成，则根据当前的播放模式来决定下一首歌的播放
    static bool last_song_finished = false;
    if(music_play::instance().song_finished && !last_song_finished)
    {
        if(_ctx._playing_mode == MusicPlayingMode::CIRCLE)
        {
            //单曲循环
            music_play::instance().set_same_song();
        }
        else if(_ctx._playing_mode == MusicPlayingMode::SEQUENTIAL)
        {
           menu::_ctx.current_music_index = (_ctx.current_music_index + 1) % MUSIC_MENU_MAX_NUM;
        }
        else if(_ctx._playing_mode == MusicPlayingMode::RANDOM)
        {
            menu::_ctx.current_music_index = HAL_GetTick() % MUSIC_MENU_MAX_NUM;
        }
        else if(_ctx._playing_mode == MusicPlayingMode::ONCE)
        {
            menu::_ctx.current_playing_state = MusicPlayingState::STOP;
        }
    }
    last_song_finished = music_play::instance().song_finished;
    
    
    menuFsm.execute(&_ctx);
}
