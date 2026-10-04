#include "song.h"

#if FLASH_WRITE_MODE

#include "song_lists.h"

#else

#include "flash_song_lists.h"

#endif

void music_play::BuzzerSong::enter(song_ctx* ctx)
{

}

void music_play::BuzzerSong::execute(song_ctx* ctx)
{
    static int8_t last_music_index = -1;
    if(ctx->cmd.current_music_index != last_music_index)
    {
        music_play::instance().set_song(&song_list[ctx->cmd.current_music_index]);
        last_music_index = ctx->cmd.current_music_index;
    }
    if(ctx->cmd.current_playing_state == song_ctx::song_cmd::playing_state::PLAYING)
    {
        if(!music_play::instance()._ctx.song_finished)
        {
            music_play::instance().play_music(ctx->cmd.rate*0.1f);
        }
        else 
        {
            music_play::instance().keep_silent();
        }
    }
    else if(ctx->cmd.current_playing_state == song_ctx::song_cmd::playing_state::STOP ||
             ctx->cmd.current_playing_state == song_ctx::song_cmd::playing_state::IDLE)
    {
        music_play::instance().keep_silent();
    }

    music_play::instance().set_final_volume(ctx->cmd.volume);
    music_play::instance().set_output();


}

void music_play::BuzzerSong::exit(song_ctx* ctx)
{
    music_play::instance().keep_silent();
    music_play::instance().set_output();
}