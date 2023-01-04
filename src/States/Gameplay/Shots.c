#include "Internal.h"

struct InvaderShot* invaderShots = NULL;
struct CannonShot* cannonShots = NULL;
int invaderShotsVel;

void InitShots()
{
    invaderShotsVel = 2;
}

void UpdateShots()
{
    for (int i = 0; i < arrlen(invaderShots); i++)
    {
        UpdateAnimation(&invaderShots[i].animation);
        if (HasAnimationFinished(&invaderShots[i].animation))
            ResetAnimation(&invaderShots[i].animation);

        invaderShots[i].y += invaderShotsVel;
    }

    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        cannonShots[i].y -= SHOT_CANNON_VEL;

        if (cannonShots[i].y <= TOURIST_Y - 8)
        {
            Animation animation;
            InitAnimation(&animation, 1, (AnimationFrame) {
                .clip = CLIP_CANNON_SHOT_EXPLOSION,
                    .timer = {
                    .reachedTimeout = false,
                    .time = 0,
                    .timeout = EXPLOSION_SHOT_TIMEOUT
                }
            });
            AddExplosion(cannonShots[i].x - 3, cannonShots[i].y, &animation);

            // remove shot
            arrdel(cannonShots, i);
        }
    }
}

void RenderShots()
{
    for (int i = 0; i < arrlen(invaderShots); i++)
    {
        RenderAnimation(
            invaderShots[i].x,
            invaderShots[i].y,
            &invaderShots[i].animation
        );
    }

    for (int i = 0; i < arrlen(cannonShots); i++)
        RenderClip(cannonShots[i].x, cannonShots[i].y, CLIP_CANNON_SHOT);
}