#ifndef TIMER_H
#define TIMER_H

#include <SDL_types.h>
#include <stdbool.h>

// This kind of thing is appearing too much.
struct Timer
{
	bool reachedTimeout; // read-only
	Uint32 time;         // read-only
	Uint32 timeout;
};

void UpdateTimer(struct Timer* timer);


#endif // TIMER_H
