#include "component/buzzer/buzzer.h"
#include "screen.h"

void screen::PlayingMusicScreen::enter(screen_ctx* ctx)
{
    
}

void screen::PlayingMusicScreen::execute(screen_ctx* ctx)
{

    // 歌名
    pen::instance()
    .set_position(0, 10)
    .draw_string(song_list[ctx->music_index].song_name, 0, 2);

    //进度条
    int overall_time = music_play::instance().get_current_song_overall_time();
    int current_time = music_play::instance().get_current_song_current_time();
    pen::instance()
    .set_position(10, 25)
    .draw_rectangle(108, 1, 2)
    .set_position(10, 24)
    .draw_rectangle(108*((float)current_time/(float)overall_time), 3, 2);

    // 当前播放时间  /  总时间
    int overall_time_second = ((overall_time + 999) % 60000) /1000;
    int overall_time_minute = (overall_time + 999) / 60000;//分钟
    int current_time_second = ((current_time + 999) % 60000) /1000;
    int current_time_minute = (current_time + 999) / 60000;//分钟
    char time_context[25] = {0};
    sprintf(time_context, "%d:%02d/%d:%02d", current_time_minute, current_time_second, overall_time_minute, overall_time_second);
    
    pen::instance()
    .set_position(10, 36)
    .draw_string(time_context, 0, 2);

    // 是否播放
    if(ctx->playing_state == menu::MusicPlayingState::PLAYING)
    {
        pen::instance()
        .set_position(34, 46)
        .draw_string("Playing...");
    }
    else if(ctx->playing_state == menu::MusicPlayingState::STOP)
    {
        pen::instance()
        .set_position(43, 46)
        .draw_string("S.T.O.P");
    }
    //播放模式
    pen::instance()
    .set_position(10, 56);
    if(ctx->playing_mode == menu::MusicPlayingMode::CIRCLE)
    {
        pen::instance().draw_string("MODE : CIRCLE", 0, 2);
    }
    else if(ctx->playing_mode == menu::MusicPlayingMode::ONCE)
    {
        pen::instance().draw_string("MODE : ONCE", 0, 2);
    }
    else if(ctx->playing_mode == menu::MusicPlayingMode::SEQUENTIAL)
    {
        pen::instance().draw_string("MODE : SEQUENTIAL", 0, 2);
    }
    else if(ctx->playing_mode == menu::MusicPlayingMode::RANDOM)
    {
        pen::instance().draw_string("MODE : RANDOM", 0, 2);
    }
}