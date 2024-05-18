#ifndef _GLOBALS_
#define _GLOBALS_

#include "Timer.h"

#ifndef EXT
#define EXT(type, name, val) extern type name
#endif

#ifndef EXT_NOINIT
#define EXT_NOINIT(type, name) extern type name
#endif

EXT(float, fps, 0);
EXT(bool, viewFps, true);
EXT_NOINIT(Timer, fpsViewClock);

int credits, score, hiScore;

#undef EXT

#undef EXT_NOINIT

#endif /* _GLOBALS_ */
