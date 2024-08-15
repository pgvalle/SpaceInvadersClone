#ifndef EXPLOSIONS_H
#define EXPLOSIONS_H

#include "explosion.h"

DECL(std::vector<Explosion *>, explosions);

void updateExplosions();
void renderExplosions();

#endif // EXPLOSIONS_H
