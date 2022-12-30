#include "Entities.h"
#include "../Application.h"

void InitCannon(struct Cannon* cannon)
{
	cannon->deathTex = ENTITYTEX_CANNON_DEAD0;

	cannon->x = CANNON_X_INIT;

    cannon->dead = false;
    cannon->deaths = 0;
    cannon->deathTimer = 0;
    cannon->deathTexSwapTimer = 0;

    cannon->shooting = false;
    cannon->shotTimer = 0;
}

void UpdateCannon(struct Cannon* cannon)
{
	cannon->shooting = false; // assume cannon isn't shooting

    if (cannon->dead)
    {
        // update death animation
        cannon->deathTexSwapTimer += app.frameTime;
        if (cannon->deathTexSwapTimer >= CANNON_DEATH_TEX_SWAP_TIMEOUT)
        {
            if (cannon->deathTex == ENTITYTEX_CANNON_DEAD1)
            	cannon->deathTex = ENTITYTEX_CANNON_DEAD0;
            else
            	cannon->deathTex = ENTITYTEX_CANNON_DEAD1;

            cannon->deathTexSwapTimer = 0;
        }
        // update death
        cannon->deathTimer += app.frameTime;
        if (cannon->deathTimer >= CANNON_DEATH_TIMEOUT)
        {
        	cannon->dead = false;
            cannon->deaths++;
            // reset death timer stuff entirely
            cannon->deathTex = ENTITYTEX_CANNON_DEAD0;
            cannon->deathTimer = 0;
            cannon->deathTexSwapTimer = 0;
        }
    }
    else // not dead
    {
        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_SPACE])
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
        if (cannon->shotTimer < CANNON_SHOT_COOLDOWN)
            cannon->shotTimer += app.frameTime;
        else if (keys[SDL_SCANCODE_SPACE])
        {
            cannon->shotTimer = 0;
            cannon->shooting = true;
        }
    }
}

void RenderCannon(struct Cannon* cannon)
{
	if (!cannon->dead)
        RenderEntity(cannon->x, CANNON_Y, ENTITYTEX_CANNON);
    else if (cannon->deathTimer < CANNON_DEATH_TIMEOUT)
        RenderEntity(cannon->x, CANNON_Y, cannon->deathTex);
}
