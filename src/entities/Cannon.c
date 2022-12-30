#include "Entities.h"
#include "../Application.h"

void InitCannon(struct Cannon* cannon)
{
	cannon->deathTex = ENTITYTEX_CANNON_DEAD0;

	cannon->x = CANNON_X_INIT;

    cannon->dead = false;
    cannon->deaths = 0;
    cannon->deathTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = CANNON_DEATH_TIMEOUT
    };
    cannon->deathTexSwapTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = CANNON_DEATH_TEX_SWAP_TIMEOUT
    };

    cannon->shooting = false;
    cannon->shotTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = CANNON_SHOT_COOLDOWN
    };
}

void UpdateCannon(struct Cannon* cannon)
{
	cannon->shooting = false; // assume cannon isn't shooting

    if (cannon->dead)
    {
        // update death status
        UpdateTimer(&cannon->deathTimer);
        if (cannon->deathTimer.reachedTimeout)
        {
            cannon->dead = false;
            cannon->deaths++;
            // first death animation frame is this
            cannon->deathTex = ENTITYTEX_CANNON_DEAD0;
            // reset deathTexSwapTimer
            cannon->deathTexSwapTimer.time = 0;
        }
        else // update death animation
        {
            UpdateTimer(&cannon->deathTexSwapTimer);
            if (cannon->deathTexSwapTimer.reachedTimeout)
            {
                if (cannon->deathTex == ENTITYTEX_CANNON_DEAD1)
                    cannon->deathTex = ENTITYTEX_CANNON_DEAD0;
                else
                    cannon->deathTex = ENTITYTEX_CANNON_DEAD1;
            }
        }
    }
    else // not dead
    {
        // assume cannon isn't shooting
        cannon->shooting = false;

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_RETURN])
        {
        	cannon->dead = true;
        	return;
        }

        // update position with keyboard input
        if (keys[SDL_SCANCODE_LEFT])
            cannon->x -= 1;
        if (keys[SDL_SCANCODE_RIGHT])
            cannon->x += 1;

        // update shot mechanic
        if (!cannon->shotTimer.reachedTimeout)
            UpdateTimer(&cannon->shotTimer);
        else if (keys[SDL_SCANCODE_SPACE])
        {
            cannon->shotTimer.reachedTimeout = false;
            cannon->shooting = true;
        }
    }
}

void RenderCannon(struct Cannon* cannon)
{
	if (!cannon->dead)
        RenderEntity(cannon->x, CANNON_Y, ENTITYTEX_CANNON);
    else if (!cannon->deathTimer.reachedTimeout)
        RenderEntity(cannon->x, CANNON_Y, cannon->deathTex);
}
