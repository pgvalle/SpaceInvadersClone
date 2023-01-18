/**
 * Mersenne Twister
 * Implemented by: https://github.com/ESultanik/mtwister
 * Originaly developed by: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html
*/
#ifndef UTILS_RANDOM_H
#define UTILS_RANDOM_H

#include <stdlib.h>
#include <time.h>

static inline
int random_get()
{
    return rand();
}

// returns an integer in the range [a, b]
static inline
int random_get_range(int a, int b)
{
    const int min = a < b ? a : b;
    const int max = a > b ? a : b;
    return rand() % (max - min + 1) + min;
}

// returns a pseudo-random float in the range [0..1].
static inline
float random_get_float()
{
    return (float)rand() / RAND_MAX;
}

// returns a pseudo-random float in the range [a..b].
static inline
float random_get_float_range(float a, float b)
{
    const float min = a < b ? a : b;
    const float max = a > b ? a : b;
    return random_get_float() * (max - min) + min;
}


#endif // UTILS_RANDOM_H
