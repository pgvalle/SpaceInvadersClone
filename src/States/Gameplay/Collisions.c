#include "Internal.h"

void ProcessTouristCollision()
{
    // if dead or didn't spawn don't check for collisions
    if (tourist.dead || !tourist.spawned)
        return;

    const SDL_Rect touristRect = { (int)roundf(tourist.x) + 4, TOURIST_Y, 16, 8 };

    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        const SDL_Rect shotRect = { cannonShots[i].x, cannonShots[i].y, 1, 4 };
        if (SDL_HasIntersection(&shotRect, &touristRect)) // collided
        {
            // create explosion animation
            Animation animation;
            InitAnimation(&animation, 1, (AnimationFrame){
                .clip = CLIP_TOURIST_EXPLOSION,
                    .timer = {
                    .has_timed_out = false,
                    .time = 0,
                    .timeout = EXPLOSION_TOURIST_TIMEOUT
                }
            });
            AddExplosion((int)tourist.x, TOURIST_Y, &animation);

            tourist.dead = true;

            // remove shot from list
            arrdel(cannonShots, i);
        }
    }
}

void ProcessHordeCollisions()
{
    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        const SDL_Rect shotRect = { cannonShots[i].x, cannonShots[i].y, 1, 4 };

        for (int j = 0; j < HORDE_INVADER_COUNT; j++)
        {
            if (horde.invaders[j].dead)
                continue;

            const int invaderX = horde.invaders[j].x;
            const int invaderY = horde.invaders[j].y;
            SDL_Rect invaderRect = { invaderX, invaderY, 12, 8 };
            if (j / 11 == 0)
                invaderRect = (SDL_Rect){ invaderX + 2, invaderY, 8, 8 };
            else if (j / 11 <= 2)
                invaderRect = (SDL_Rect){ invaderX + 1, invaderY, 11, 8 };

            if (SDL_HasIntersection(&shotRect, &invaderRect))
            {
                // create explosion
                Animation animation;
                InitAnimation(&animation, 1, (AnimationFrame){
                    .clip = CLIP_INVADER_EXPLOSION,
                        .timer = {
                        .has_timed_out = false,
                        .time = 0,
                        .timeout = EXPLOSION_INVADER_TIMEOUT
                    }
                });
                AddExplosion(invaderX, invaderY, &animation);

                horde.invaders[j].dead = true;
                // increate horde speed
                if (horde.moveTimer.timeout > 16)
                    horde.moveTimer.timeout -= 16;

                // This creates that delay effect in horde movement when an invader gets shot
                if (horde.moveTimer.timeout < horde.moveTimer.time + EXPLOSION_INVADER_TIMEOUT)
                {
                    if (horde.moveTimer.time >= EXPLOSION_INVADER_TIMEOUT)
                        horde.moveTimer.time -= EXPLOSION_INVADER_TIMEOUT;
                    else
                        horde.moveTimer.time = 0;
                }

                // remove shot from list
                arrdel(cannonShots, i);
            }
        }
    }
}

void ProcessBunkersCollisions()
{

}

void ProcessCannonCollision()
{

}