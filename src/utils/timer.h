#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

// This kind of thing is appearing too much.
typedef struct timer_t {
	bool has_timed_out; // READ-ONLY
	uint32_t time;      // READ-ONLY
	uint32_t timeout;   // set with timer_set_timeout
} timer_t;

void timer_update(timer_t* timer);

static inline void timer_reset(timer_t* timer)
{
	timer->has_timed_out = false;
	timer->time = 0;
}

static inline void timer_set_timeout(timer_t* timer, uint32_t timeout)
{
	timer_reset();
	timer->timeout = timeout;
}


#endif // TIMER_H
