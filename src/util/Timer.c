#include "Timer.h"
#include "../Application.h"

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
