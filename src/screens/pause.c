#include "internal.h"
#include "../score.h"

void process_pause_event(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat ||
        pause.state == PAUSE_RESUMING)
        return;
    
    switch (event->key.keysym.sym) {
    case SDLK_m:
        screen = SCREEN_MENU;
        score = 0;
        reset_menu();
        break;
    case SDLK_ESCAPE:
        pause.state = PAUSE_RESUMING;
        pause.timer = 0;
        break;
    case SDLK_q:
        screen = SCREEN_EXIT;
        reset_over();
        break;
    }
}

void update_pause(Uint32 delta)
{
    switch (pause.state) {
    case PAUSE_BLINKING_ON:
    case PAUSE_BLINKING_OFF:
        pause.timer += delta;
        if (pause.timer >= 498) {
            pause.state = (pause.state == PAUSE_BLINKING_ON ?
                PAUSE_BLINKING_OFF : PAUSE_BLINKING_ON);
            pause.timer = 0;
        }
        break;
    case PAUSE_RESUMING:
        pause.timer += delta;
        if (pause.timer >= 3008) // 16 * 188 (3 seconds)
            screen = SCREEN_PLAY;
        break;
    }
}

void render_pause()
{
    // make background darker. It feels like game is really paused
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 225);
    const SDL_Rect overlay_rect = { 0, 0, WIDTH, HEIGHT };
    SDL_RenderFillRect(ren, &overlay_rect);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);

    switch (pause.state) {
    case PAUSE_BLINKING_ON:
        render_text("<ESC> RESUME", 12, 64, 64);
        render_text("<M> MENU", 8, 80, 80);
        render_text("<Q> QUIT", 8, 80, 96);
    case PAUSE_BLINKING_OFF:
        render_score_advances_table();
        break;
    case PAUSE_RESUMING: {
        int countdown = 3 - (int)pause.timer / 1000;
        char countdown_text[3];
        sprintf(countdown_text, "%02d", countdown == 0 ? 1 : countdown);
        render_text(countdown_text, 2, WIDTH / 2 - 8, 112);
        break; }
    }
}

struct pause_screen_t pause;