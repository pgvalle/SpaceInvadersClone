#include "internal.h"
#include "../score.h"
#include "../credit.h"

void reset_menu()
{
    menu.state = MENU_DISPLAYING;
    menu.display_i = 0;
    menu.timer = 0;
}

void process_menu_event(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN || event->key.repeat) {
        return;
    }
    
    const SDL_Keycode key = event->key.keysym.sym;
    switch (menu.state) {
    case MENU_DISPLAYING:
    case MENU_WAITING:
        if (key == SDLK_RETURN) {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        if (key == SDLK_p) {
            screen = SCREEN_PLAY;
            remove_credit();
            reset_play();
        }
        else if (key == SDLK_q)
            screen = SCREEN_EXIT;
        break;
    }
}

void update_menu(Uint32 delta)
{
    switch (menu.state) {
    case MENU_DISPLAYING:
        menu.timer += delta;
        if (menu.timer >= 160) {
            if (++menu.display_i == 14)
                menu.state = MENU_WAITING;
            menu.timer = 0;
        }
        break;
    case MENU_WAITING:
        menu.timer += delta;
        if (menu.timer >= 1008) {
            menu.state = MENU_BLINKING_ON;
            menu.timer = 0;
        }
        break;
    case MENU_BLINKING_ON:
    case MENU_BLINKING_OFF:
        menu.timer += delta;
        if (menu.timer >= 498) {
            menu.state = (menu.state == MENU_BLINKING_ON ?
                MENU_BLINKING_OFF : MENU_BLINKING_ON);
            menu.timer = 0;
        }
        break;
    }
}

void render_menu()
{
    switch (menu.state) {
    case MENU_BLINKING_ON:
        render_text("<P> PLAY", 8, 80, 80);
        render_text("<Q> QUIT", 8, 80, 96);
    case MENU_BLINKING_OFF:
        render_text("SPACE INVADERS", 14, 56, 56);
        render_score_advances_table();
        break;
    case MENU_WAITING:
    case MENU_DISPLAYING:
        render_text("SPACE INVADERS", menu.display_i, 56, 56);
        break;
    }
}

struct menu_screen_t menu;