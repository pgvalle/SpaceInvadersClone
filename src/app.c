#include "app.h"
#include "credit_coins.h"
#include "score.h"

#include "../stb_image.h"

#include <stdlib.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////////
// UTILS //
///////////////////////////////////////////////////////////////////////////////

void render_text(const char* text, int len, int x, int y)
{
    const static char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>=";
    const static int CHARS_LEN_WRAP = 6;
    for (int i = 0; i < len; i++) {
        const char c_upper = toupper(text[i]);
        for (int j = 0; j < sizeof(CHARS) - 1; j++) {
            if (c_upper == CHARS[j]) {
                const SDL_Rect clip = { TILE * (j % CHARS_LEN_WRAP),
                    40 + TILE * (j / CHARS_LEN_WRAP), TILE, TILE };

                render_clip(&clip, x, y);
                break;
            }
        }

        x += TILE;
    }
}

///////////////////////////////////////////////////////////////////////////////
// ASSET MANAGEMENT //
///////////////////////////////////////////////////////////////////////////////

#define RESOURCES "../res"

void load_app_resources()
{
    int width = 48, height = 96;
    void* pixels = stbi_load(RESOURCES "/atlas.png", &width, &height, NULL, 4);
    if (pixels == NULL) {
        const SDL_Rect rect = { 0, 0, width, height };
        SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height, 24,
            0xff0000, 0x00ff00, 0x0000ff, 0);
        
        SDL_FillRect(surface, &rect, 0xffffff);
        atlas = SDL_CreateTextureFromSurface(ren, surface);
        SDL_FreeSurface(surface);
    } else {
        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, width, height,
            32, 4*width, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
        atlas = SDL_CreateTextureFromSurface(ren, surface);
        stbi_image_free(pixels);
        SDL_FreeSurface(surface);
    }
}

void free_app_resources()
{
    SDL_DestroyTexture(atlas);
}

///////////////////////////////////////////////////////////////////////////////
// MAINLOOP STUFF //
///////////////////////////////////////////////////////////////////////////////

// #include "screens/menu.h"

void process_app_events(const SDL_Event* event)
{
    if (event->type == SDL_QUIT) {
        screen = SCREEN_EXIT;
    } else {
        process_credit_coin_events(&event);
        switch (screen) {
        case SCREEN_MENU:
            // process_menu_events(event);
            break;
        case SCREEN_PLAY:
            // process_play_events(event);
            break;
        case SCREEN_PAUSE:
            // process_pause_events(event);
            break;
        case SCREEN_GAMEOVER:
            // process_over_events(event);
            break;
        }
    }
}

void update_app(Uint32 delta)
{
    /*switch (screen) {
    case SCREEN_PAUSE: {
        update_pause(delta);
        break;
    }
    case SCREEN_MENU: {
        update_menu(delta);
        break;
    }
    case SCREEN_PLAY: {
        update_play(delta);
        break;
    }
    case SCREEN_GAMEOVER: {
        update_over(delta);
        break;
    }}*/
}

void render_app()
{
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
            
    /*switch (screen) {
        case SCREEN_PAUSE: {
            render_play();
            render_pause();
            break;
        }
        case SCREEN_MENU: {
            render_menu();
            break;
        }
        case SCREEN_PLAY: {
            render_play();
            break;
        }
        case SCREEN_GAMEOVER: {
            render_play();
            render_over();
            break;
        }
    }
*/
    render_score_counters();
    render_credit_coin_counter();

    SDL_RenderPresent(ren);
}

void run_app_loop()
{
    const Uint32 FPS = 60;
    Uint32 start = 0, delta = 0, event_start = 0, event_wait = 1000 / FPS;

    while (screen != SCREEN_EXIT) {
        SDL_Event event;
        if (SDL_WaitEventTimeout(&event, event_wait)) {
            process_app_events(&event);

            const Uint32 event_delta = SDL_GetTicks() - event_start;
            event_start += event_delta;
            // wait less next time
            event_wait -= event_delta < event_wait ? event_delta : event_wait;
        } else {
            update_app(delta);
            render_app();
        
            delta = SDL_GetTicks() - start;
            start += delta;
            // reset event timing
            event_start = start;
            event_wait = 1000 / FPS;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// ENTRYPOINT //
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** args)
{
    const int INITIAL_SCALE = 2;

    // INITIALIZATION //

    SDL_Init(SDL_INIT_EVERYTHING);
    // Mix_Init(MIX_INIT_MP3);
    srand(time(NULL));

    SDL_ShowCursor(SDL_FALSE); // retro games don't have a cursor showing up
    // they don't grab any sort of mouse events as well.
    for (SDL_EventType i = SDL_MOUSEMOTION; i <= SDL_MOUSEWHEEL; i++) {
        SDL_EventState(i, SDL_DISABLE);
    }

    screen = SCREEN_MENU;
    win = SDL_CreateWindow("Space Invaders Clone", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, INITIAL_SCALE * WIDTH, INITIAL_SCALE * HEIGHT,
        SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_MOUSE_FOCUS);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(ren, INITIAL_SCALE, INITIAL_SCALE);
    credit_coins = 1;
    score = high_score = 0;

    load_app_resources();
    
    run_app_loop(); // EXECUTION //

    // TERMINATION //

    free_app_resources();

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// APP STATE //
///////////////////////////////////////////////////////////////////////////////

enum screen_t screen;
SDL_Window* win;
SDL_Renderer* ren;

SDL_Texture* atlas;
