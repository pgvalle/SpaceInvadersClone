#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

// This kind of thing is appearing too much.
typedef struct timer_t {
	bool has_timed_out;
	uint32_t time;
	uint32_t timeout;
} timer_t;

void timer_update(timer_t* timer);


#endif // TIMER_H
