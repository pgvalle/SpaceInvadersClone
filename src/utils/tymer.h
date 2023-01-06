#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

// This kind of thing is appearing too much.
typedef struct tymer_t {
	bool has_timed_out; // READ-ONLY
	uint32_t time;      // READ-ONLY
	uint32_t timeout;   // set with timer_set_timeout
} tymer_t;

void timer_update(tymer_t* timer);

static inline void timer_reset(tymer_t* timer)
{
	timer->has_timed_out = false;
	timer->time = 0;
}

static inline void timer_set_timeout(tymer_t* timer, uint32_t timeout)
{
	timer_reset(timer);
	timer->timeout = timeout;
}


#endif // TIMER_H
