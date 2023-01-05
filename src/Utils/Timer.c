#include "Timer.h"
#include "../Application.h"

void timer_update(timer_t* timer)
{
	// assume timer didn't reach timeout yet
	timer->has_timed_out = false;

	// update timer in fact
	timer->time += app.frameTime;
	if (timer->time >= timer->timeout)
	{
		timer->time = 0;
		// valid only until next call to timer_update
		timer->has_timed_out = true;
	}
}