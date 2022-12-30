#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

// This kind of problem is appearing too much.
struct Timer
{
	bool reachedTimeout; // read-only
	int time;            // read-only
	int timeout;
};

void UpdateTimer(struct Timer* timer);


#endif // TIMER_H
