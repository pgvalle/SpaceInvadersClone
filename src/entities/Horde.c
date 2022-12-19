#include "Horde.h"

void Horde_Initialize(struct Horde* horde)
{
    for (int i = 0; i < 11; i++)
    {
        // first row
        horde->invaders[i].type = INVADER_0;
        horde->invaders[i].x = HORDE_X_INIT + 2 + 16 * i;
        horde->invaders[i].y = HORDE_Y_INIT;
        // second
        horde->invaders[11 + i].type = INVADER_1;
        horde->invaders[11 + i].x = HORDE_X_INIT + 1 + 16 * i;
        horde->invaders[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde->invaders[22 + i].type = INVADER_1;
        horde->invaders[22 + i].x = HORDE_X_INIT + 1 + 16 * i;
        horde->invaders[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde->invaders[33 + i].type = INVADER_2;
        horde->invaders[33 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde->invaders[44 + i].type = INVADER_2;
        horde->invaders[44 + i].x = HORDE_X_INIT + 16 * i;
        horde->invaders[44 + i].y = HORDE_Y_INIT + 64;
    }

    for (int i = 0; i < HORDE_SIZE + 1; i++)
    {
        horde->invaders[i].moveAnimDone = false;
        horde->invaders[i].alive = true;
        horde->invaders[i].deathAnimTime = INVADER_DEATH_ANIM_TIMEOUT;
        horde->invaders[i].moveAnimFrame = 1;
    }

    horde->locked = false;
    horde->goingRight = true;
    horde->moveCount = 8;
    horde->moveTimer = 0;
    horde->moveTimeout = HORDE_MOVE_DELTA_INIT;
}

void Horde_Move(struct Horde* horde, int delta)
{
    // update movement timer
    horde->moveTimer += delta;
    // can't move yet
    if (horde->locked || horde->moveTimer < horde->moveTimeout)
        return;

    // moved 16 times horizontaly. Move down now.
    if (horde->moveCount == 16)
    {
        for (int i = 0; i < HORDE_SIZE; i++)
        {
            // We only care about alive invaders here.
            if (horde->invaders[i].alive)
                horde->invaders[i].y += 8;
        }

        horde->moveCount = 0; // reset move counter
    }
    else
    {
        int offset = horde->goingRight ? 2 : -2;
        for (int i = 0; i < HORDE_SIZE; i++)
        {
            if (horde->invaders[i].alive)
                horde->invaders[i].x += offset;
        }

        horde->moveCount++;
    }
}

void Horde_Die(struct Horde* horde, int delta)
{
    // assume horde should be locked
    horde->locked = true;

    int deadInvadersCount = 0;
    int deadAndTimedInvadersCount = 0;
    for (int i = 0; i < HORDE_SIZE; i++)
    {
        // Ignore alive invaders.
        if (horde->invaders[i].alive)
            continue;

        deadInvadersCount++;
        if (horde->invaders[i].deathTimer < INVADER_DEATH_TIMEOUT)
            horde->invaders[i].deathTimer += delta;
        else
            deadAndTimedInvadersCount++;
    }

    // if this doesn't happen. It means the horde should indeed be locked
    if (deadInvadersCount == deadAndTimedInvadersCount)
        horde->locked = false;
}

void Horde_TimeUpdate(struct Horde* horde, int delta)
{
    Horde_Move(horde, delta);
    Horde_Die(horde, delta);
}

void Horde_Render(const struct Horde* horde, SDL_Renderer* renderer)
{
    
}