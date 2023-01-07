/**
 * Mersenne Twister
 * Implemented by: https://github.com/ESultanik/mtwister
 * Originaly developed by: http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/emt.html
*/
#ifndef RANDOM_H
#define RANDOM_H

#include <stdint.h>

#define STATE_VECTOR_LENGTH 624
#define STATE_VECTOR_M      397 /* changes to STATE_VECTOR_LENGTH also require changes to this */

typedef struct random_t {
  uint32_t mt[STATE_VECTOR_LENGTH];
  int index;
} random_t;

void random_init(random_t* rand, uint32_t seed);
// returns a integer in the range [0, 4294967296]
// or [-2147483647, 2147483648] if you will
uint32_t random_get_uint32(random_t* rand);
// Generates a pseudo-random float in the range [0..1].
float random_get_float(random_t* rand);


#endif // RANDOM_H
