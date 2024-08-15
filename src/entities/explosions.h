#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include "explosion.h"

extern std::vector<Explosion *> explosions;

void updateExplosions();
void renderExplosions();

#endif // EXPLOSIONS_H
