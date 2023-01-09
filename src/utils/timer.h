#ifndef UTILS_TIMER_H
#define UTILS_TIMER_H

#include <stdint.h>
#include <stdbool.h>

// This kind of thing is appearing too much.
typedef struct t1mer_t
{
	bool has_timed_out; // READ-ONLY
	uint32_t time;      // READ-ONLY
	uint32_t timeout;   // set with timer_set_timeout
} t1mer_t;

void timer_update(t1mer_t* timer);

static inline
void timer_reset(t1mer_t* timer)
{
	timer->has_timed_out = false;
	timer->time = 0;
}

static inline void
timer_set_timeout(t1mer_t* timer, uint32_t timeout)
{
	timer_reset(timer);
	timer->timeout = timeout;
}


#endif // UTILS_TIMER_H
