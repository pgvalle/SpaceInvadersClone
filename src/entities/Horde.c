#include "Entities.h"
#include "../Application.h"
#include "../Utils/Renderer.h"

void InitHorde(struct Horde* horde)
{    
    for (int i = 0; i < 11; i++)
    {
        // first row
        horde->invaders[i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[i].y = HORDE_Y_INIT;
        // second
        horde->invaders[11 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde->invaders[22 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde->invaders[33 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde->invaders[44 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[44 + i].y = HORDE_Y_INIT + 64;
    }

    // All invaders start dead to play an animation before starting gameplay
    for (int i = 0; i < INVADER_COUNT; i++)
    {
        horde->invaders[i].dead = true;
        horde->invaders[i].deathTimer = (struct Timer){
            .reachedTimeout = true,
            .time = 0,
            .timeout = INVADER_DEATH_TIMEOUT
        };
    }

    horde->deadCount = INVADER_COUNT;

    horde->row0Tex  = ENTITYTEX_INVADER00;
    horde->row12Tex = ENTITYTEX_INVADER10;
    horde->row34Tex = ENTITYTEX_INVADER20;

    horde->whoShot = -1;

    horde->moveRight = true;
    horde->moveCount = 8;
    horde->moveTimer = (struct Timer){
        .reachedTimeout = false,
        .time = 0,
        .timeout = HORDE_STAND_TIMEOUT_INIT
    };
}

void UpdateAliveInvaders(struct Horde* horde)
{
    // update move timer
    UpdateTimer(&horde->moveTimer);
    // can't move yet
    if (!horde->moveTimer.reachedTimeout)
        return;

    // should move down
    if (horde->moveCount == 16)
    {
        for (int i = 0; i < INVADER_COUNT; i++)
        {
            if (!horde->invaders[i].dead)
                horde->invaders[i].y += 8;
        }

        horde->moveCount = 0; // reset move counter
        horde->moveTimer.timeout; // increase invaders speed somehow
        horde->moveRight = !horde->moveRight; // change horizontal movement
    }
    else
    {
        int offset = horde->moveRight ? 2 : -2;
        for (int i = 0; i < INVADER_COUNT; i++)
        {
            if (!horde->invaders[i].dead)
                horde->invaders[i].x += offset;
        }

        horde->moveCount++;
    }

    // change animation frame for all invaders in horde
    if (horde->row0Tex  == ENTITYTEX_INVADER00)
    {
        horde->row0Tex  = ENTITYTEX_INVADER01;
        horde->row12Tex = ENTITYTEX_INVADER11;
        horde->row34Tex = ENTITYTEX_INVADER21;
    }
    else
    {
        horde->row0Tex  = ENTITYTEX_INVADER00;
        horde->row12Tex = ENTITYTEX_INVADER10;
        horde->row34Tex = ENTITYTEX_INVADER20;
    }
}

bool UpdateDeadInvader(struct Horde* horde)
{
    bool invadersDying = false;

    for (int i = 0; i < INVADER_COUNT; i++)
    {
        // don't care
        if (!horde->invaders[i].dead)
            continue;

        // update death timer
        UpdateTimer(&horde->invaders[i].deathTimer);
        if (!horde->invaders[i].deathTimer.reachedTimeout)
            invadersDying = true;
    }

    return !invadersDying; // !invadersDying = may update alive invaders
}

void UpdateHorde(struct Horde* horde)
{
    const bool finished = UpdateDeadInvader(horde);
    if (finished)
	   UpdateAliveInvaders(horde);

    // accelerate invaders
}

void RenderHorde(const struct Horde* horde)
{
	for (int i = 0; i < INVADER_COUNT; i++)
	{
		const int x = horde->invaders[i].x;
        const int y = horde->invaders[i].y;

        if (!horde->invaders[i].dead)
        {
            switch (i / 11)
            {
            case 0: // first row
                RenderEntityTex(x, y, horde->row0Tex);
            	break;
            case 1: case 2: // second or third
                RenderEntityTex(x, y, horde->row12Tex);
            	break;
            case 3: case 4: // fourth or fifth
                RenderEntityTex(x, y, horde->row34Tex);
            	break;
            }
        }
        else if (!horde->invaders[i].deathTimer.reachedTimeout)
            RenderEntityTex(x - 1, y, ENTITYTEX_INVADER_DEAD);
	}
}