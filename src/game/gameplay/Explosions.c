#include "Internal.h"

struct Explosion* explosions = NULL;

void AddExplosion(int x, int y, const animation_t* anim)
{
    const struct Explosion explosion = { .x = x, .y = y, .animation = *anim };
    arrput(explosions, explosion);
}

void UpdateExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        animation_update(&explosions[i].animation);
        if (animation_is_over(&explosions[i].animation))
        {
            animation_free(&explosions[i].animation);
            arrdel(explosions, i);
        }
    }
}

void RenderExplosions()
{
    for (int i = 0; i < arrlen(explosions); i++)
    {
        animation_render(
            &explosions[i].animation,
            explosions[i].x,
            explosions[i].y
        );
    }
}