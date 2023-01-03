#include "Internal.h"

struct Explosion* explosions = NULL;

void AddExplosion(int x, int y, const struct Animation* anim)
{
    const struct Explosion explosion = { .x = x, .y = y, .animation = *anim };
    arrput(explosions, explosion);
}

void UpdateExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        UpdateAnimation(&explosions[i].animation);
        if (HasAnimationFinished(&explosions[i].animation))
        {
            FreeAnimation(&explosions[i].animation);
            arrdel(explosions, i);
        }
    }
}

void RenderExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        RenderAnimation(
            explosions[i].x,
            explosions[i].y,
            &explosions[i].animation
        );
    }
}