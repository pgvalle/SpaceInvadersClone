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
            invaderShots[i].x,
            invaderShots[i].y
        );
    }

    const SDL_Rect clip = { 39, 18,  1,  4 };

    for (int i = 0; i < arrlen(cannonShots); i++)
    {
        const SDL_Rect scale = {
            app.scale * cannonShots[i].x,
            app.scale * cannonShots[i].y,
            app.scale * clip.w,
            app.scale * clip.h
        };
        SDL_RenderCopy(
            app.renderer,
            asset_man_get(ASSETTYPE_TEXTURE, ATLAS_INDEX),
            &clip,
            &scale
        );
    }
}