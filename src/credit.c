#include "credit.h"
#include "app.h"

void process_credit_event(const SDL_Event* event)
{
    if (event->type != SDL_KEYDOWN) {
        return;
    }

    const SDL_Keysym keysym = event->key.keysym;
    if (keysym.sym == SDLK_c) {
        if (keysym.mod & KMOD_SHIFT) {
            remove_credit();
        } else {
            add_credit();
        }
    }
}

void render_credit_counter()
{
    static char credits_fmt[10];
    sprintf(credits_fmt, "CREDIT %02d", credits);
    render_text(credits_fmt, 9, WIDTH - 80, HEIGHT - 16);
}

int credits;