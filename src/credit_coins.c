#include "credit_coins.h"
#include "app.h"

void process_credit_coin_events(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN)
        return;

    const SDL_Keysym keysym = event->key.keysym;
    if (keysym.sym == SDLK_c) {
        if (keysym.mod == KMOD_LCTRL) {
            add_credit_coin();
        } else if (keysym.mod == KMOD_NONE) {
            remove_credit_coin();
        }
    }
}

void render_credit_coin_counter()
{
    static char credits_fmt[10];
    sprintf(credits_fmt, "CREDIT %02d", credit_coins);
    render_text(credits_fmt, 9, WIDTH - 80, HEIGHT - 16);
}

int credit_coins;