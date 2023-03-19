#include "internal.h"
#include "../credit.h"

void process_over_event(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat)
        return;

    const SDL_Keycode key = event->key.keysym.sym;
    switch (over.state) {
    case GAMEOVER_WAITING1:
        if (key == SDLK_RETURN) {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        if (key == SDLK_RETURN) {
            over.state = GAMEOVER_BLINKING_ON;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        switch (key) {
        case SDLK_RETURN:
        case SDLK_RETURN2:
            screen = SCREEN_PLAY;
            if (credits > 0)
                credits--;
            reset_play();
            break;
        case SDLK_q:
            screen = SCREEN_EXIT;
            break;
        case SDLK_m:
            screen = SCREEN_MENU;
            reset_menu();
            break;
        }
        break;
    }
}

void update_over(Uint32 delta)
{
    switch (over.state) {
    case GAMEOVER_WAITING1:
        over.timer += delta;
        if (over.timer >= 2000)
        {
            over.state = GAMEOVER_DISPLAYING;
            over.timer = 0;
        }
        break;
    case GAMEOVER_DISPLAYING:
        over.timer += delta;
        if (over.timer >= 160) {
            if (++over.display_i == 8)
                over.state = GAMEOVER_WAITING2;
            over.timer = 0;
        }
        break;
    case GAMEOVER_WAITING2:
        over.timer += delta;
        if (over.timer >= 1008) {
            over.state = GAMEOVER_BLINKING_ON;
            over.timer = 0;
        }
        break;
    case GAMEOVER_BLINKING_ON:
    case GAMEOVER_BLINKING_OFF:
        over.timer += delta;
        if (over.timer >= 498) {
            over.state = (over.state == GAMEOVER_BLINKING_ON ?
                GAMEOVER_BLINKING_OFF : GAMEOVER_BLINKING_ON);
            over.timer = 0;
        }
        break;
    }
}

void render_over()
{
    if (over.state != GAMEOVER_WAITING1) {
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 225);
        const SDL_Rect overlay_rect = { 0, 0, WIDTH, HEIGHT };
        SDL_RenderFillRect(ren, &overlay_rect);
        SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_NONE);
    }

    switch (over.state) {
    case GAMEOVER_WAITING2:
    case GAMEOVER_DISPLAYING:
        SDL_SetTextureColorMod(atlas, 216, 32, 32);
        render_text("YOU LOST", over.display_i, 80, 56);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
        break;
    case GAMEOVER_BLINKING_ON:
        render_text("<ENTER> GO AGAIN", 16, 48, 80);
        render_text("<M> MENU", 8, 80, 96);
        render_text("<Q> QUIT", 8, 80, 112);
    case GAMEOVER_BLINKING_OFF:
        SDL_SetTextureColorMod(atlas, 216, 32, 32);
        render_text("YOU LOST", 8, 80, 56);
        SDL_SetTextureColorMod(atlas, 255, 255, 255);
        break;
    }
}

struct over_screen_t over;