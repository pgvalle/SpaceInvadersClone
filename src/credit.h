#ifndef CREDIT_H
#define CREDIT_H

#include <SDL_events.h>

extern int credits;

void process_credit_events(const SDL_Event* event);
void render_credit_counter();

static inline
void add_credit()
{
    if (credits < 99) {
        credits++;
    }
}

static inline
void remove_credit()
{
    if (credits > 0) {
        credits--;
    }
}


#endif // CREDIT_H
