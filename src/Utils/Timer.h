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

void UpdateTimer(struct Timer* timer)
{
	// assume timer didn't reach timeout yet
	timer->reachedTimeout = false;

	// update timer in fact
	timer->time += app.frameTime;
	if (timer->time >= timer->timeout)
	{
		timer->time = 0;
		// valid only until next call to UpdateTimer
		timer->reachedTimeout = true;
	}
}


#endif // TIMER_H
