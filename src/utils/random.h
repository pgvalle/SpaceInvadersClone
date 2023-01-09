/**
 * Mersenne Twister
 * Implemented by: https://github.com/ESultanik/mtwister
 * Originaly developed by: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html
*/
#ifndef UTILS_RANDOM_H
#define UTILS_RANDOM_H

#include <stdint.h>

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef struct random_t
{
  uint32_t mt[STATE_VECTOR_LENGTH];
  int index;
} random_t;

void random_init(random_t* rand, uint32_t seed);
// returns an integer in the range [0, 4294967296]
// or [-2147483647, 2147483648] if you will
uint32_t random_get_uint32(random_t* rand);

// returns an integer in the range [a, b]
static inline
uint32_t random_get_uint32_range(random_t* rand, uint32_t a, uint32_t b)
{
    const int min = a < b ? a : b;
    const int max = a > b ? a : b;
    return random_get_uint32(rand) % (max - min + 1) + min;
}

// returns a pseudo-random float in the range [0..1].
float random_get_float(random_t* rand);

// returns a pseudo-random float in the range [a..b].
static inline
float random_get_float_range(random_t* rand, float a, float b)
{
    const float min = a < b ? a : b;
    const float max = a > b ? a : b;
    return random_get_float(rand) * (max - min) + min;
}


#endif // UTILS_RANDOM_H
