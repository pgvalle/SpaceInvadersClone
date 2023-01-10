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
        animation_update(&invaderShots[i].animation);
        if (animation_is_over(&invaderShots[i].animation))
            animation_reset(&invaderShots[i].animation);

        invaderShots[i].y += invaderShotsVel;
    }

    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        cannonShots[i].y -= SHOT_CANNON_VEL;

        if (cannonShots[i].y <= TOURIST_Y - 8)
        {
            animation_t animation;
            animation_init(&animation, ATLAS_INDEX, 1, (animation_frame_t) {
                .clip = { 36 ,24,  8,  8 },
                    .timer = {
                    .has_timed_out = false,
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
        animation_render(
            &invaderShots[i].animation,
            WORLD_WIDTH,
            WORLD_HEIGHT,
            invaderShots[i].x,
            invaderShots[i].y
        );
    }

    static const SDL_Rect clip = { 39, 18,  1,  4 };
    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        clip_render(
            &clip,
            ATLAS_INDEX,
            WORLD_WIDTH,
            WORLD_HEIGHT,
            cannonShots[i].x,
            cannonShots[i].y
        );
    }
}