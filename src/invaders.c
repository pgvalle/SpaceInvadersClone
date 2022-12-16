#include "gamedata.h"
#include <SDL2/SDL_image.h>


int64_t anim_timeout = 20;

SDL_Rect const death123_clip = {0, 0, 13, 8};

void invaders_reset(struct GameData *game)
{
    // Just aliases
    struct InvadersData *invaders = &game->invaders;
    struct Invader123Instance *horde = game->invaders.horde;

    for (int i = 0; i < 11; i++)
    {
        // first row
        horde[i].type = INVADER1;
        horde[i].x = HORDE_X_INIT + 2 + 16*i;
        horde[i].y = HORDE_Y_INIT;
        // second
        horde[11 + i].type = INVADER2;
        horde[11 + i].x = HORDE_X_INIT + 1 + 16*i;
        horde[11 + i].y = HORDE_Y_INIT + 16;
        // third
        horde[22 + i].type = INVADER2;
        horde[22 + i].x = HORDE_X_INIT + 1 + 16*i;
        horde[22 + i].y = HORDE_Y_INIT + 32;
        // fourth
        horde[33 + i].type = INVADER3;
        horde[33 + i].x = HORDE_X_INIT + 16*i;
        horde[33 + i].y = HORDE_Y_INIT + 48;
        // fifth
        horde[44 + i].type = INVADER3;
        horde[44 + i].x = HORDE_X_INIT + 16*i;
        horde[44 + i].y = HORDE_Y_INIT + 64;
    }

    for (int i = 0; i < HORDE_SIZE + 1; i++)
    {
        horde[i].move_anim_done = false;
        horde[i].alive = true;
        horde[i].death_anim_time = INVADER_DEATH_ANIM_TIMEOUT;
        horde[i].move_anim_frame = 1;
    }

    invaders->sideways_right = true;
    invaders->sideways_moves_count = 8;
    invaders->move_anim_timeout = HORDE_MOVE_ANIM_TIMEOUT_INIT;
    invaders->horde_locked = false;
}

void invaders_initialize(struct GameData *game)
{
    game->inv0 = IMG_LoadTexture(game->ren, "./res/img/invader0.png");
    game->inv1 = IMG_LoadTexture(game->ren, "./res/img/invader1.png");
    game->inv2 = IMG_LoadTexture(game->ren, "./res/img/invader2.png");
    game->inv3 = IMG_LoadTexture(game->ren, "./res/img/invader3.png");
    game->death123 = IMG_LoadTexture(game->ren, "./res/img/death123.png");

    invaders_reset(game);
}

void invaders_processEvents(struct GameData *game)
{
    SDL_Event event = game->event;
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            int random = rand() % 55;
            game->invaders.horde[random].alive = false;
            game->invaders.horde_locked = true;
        }
        else if (event.key.keysym.sym == SDLK_r)
        {
            invaders_reset(game);
        }
    }
}

void invaders_destroy(struct GameData *game)
{
    SDL_DestroyTexture(game->inv0);
    SDL_DestroyTexture(game->inv1);
    SDL_DestroyTexture(game->inv2);
    SDL_DestroyTexture(game->inv3);
    SDL_DestroyTexture(game->death123);
}

void invaders_animate_movement(struct GameData *game)
{
    // Just aliases
    struct InvadersData *invaders = &game->invaders;
    struct Invader123Instance *horde = game->invaders.horde;

    // update movement animation timeout
    invaders->move_anim_time -= game->frametime;
    if (invaders->move_anim_time > 0 || invaders->horde_locked) // can't animate movement yet
        return;
    invaders->move_anim_time = invaders->move_anim_timeout; // reset timeout

    // from bottom to up
    for (int i = 4; i >= 0; i--)
    {
        // from left to right
        for (int j = 0; j < 11; j++)
        {
            int k = 11 * i + j;

            // ignore horde already animated
            if (horde[k].move_anim_done)
                continue;
            
            // change movement animation frame
            horde[k].move_anim_frame = !horde[k].move_anim_frame;

            // move down if moved sideways 16 times
            if (invaders->sideways_moves_count == 16)
                horde[k].y += 8;
            else // move left or right
            {
                if (invaders->sideways_right)
                    horde[k].x += 2;
                else
                    horde[k].x -= 2;
            }

            // movement animation done for this frame
            horde[k].move_anim_done = true;
            // we only animate movement for one instance at a time to
            // reproduce the cool movement feel of the original game
            return;
        }
    }

    // All moved. Reset movement animation state
    for (int i = 0; i < HORDE_SIZE; i++)
        horde[i].move_anim_done = false;
    // increase move count and check if 16 horizontal movements were made
    if (++invaders->sideways_moves_count == 17)
    {
        invaders->sideways_right = !invaders->sideways_right;
        invaders->sideways_moves_count = 0;
    }
}

void invaders_animate_death(struct GameData *game)
{
    // Just aliases
    struct InvadersData *invaders = &game->invaders;
    struct Invader123Instance *horde = game->invaders.horde;
    int death_anim_done_count = 0;

    for (int i = 0; i < HORDE_SIZE; i++)
    {
        // only update death animation timeout for dead horde and if timeout > 0
        if (!horde[i].alive && horde[i].death_anim_time > 0)
            horde[i].death_anim_time -= game->frametime;
        else
            death_anim_done_count++;
    }

    // all death animations played. Unlock horde movement
    if (death_anim_done_count == HORDE_SIZE)
        game->invaders.horde_locked = false;
}

void invaders_update(struct GameData *game)
{
    invaders_animate_movement(game);
    invaders_animate_death(game);
}

void invader1_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        SCALING_FACTOR * 8, SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 8, 8};

    scale.x = SCALING_FACTOR * game->invaders.horde[i].x;
    scale.y = SCALING_FACTOR * game->invaders.horde[i].y;
    
    clip.x = 0;
    if (game->invaders.horde[i].move_anim_frame == 1)
        clip.x = 8;
    
    SDL_RenderCopy(game->ren, game->inv1, &clip, &scale);
}

void invader2_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        SCALING_FACTOR * 11, SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 11, 8};

    scale.x = SCALING_FACTOR * game->invaders.horde[i].x;
    scale.y = SCALING_FACTOR * game->invaders.horde[i].y;
        
    clip.x = 0;
    if (game->invaders.horde[i].move_anim_frame == 1)
        clip.x = 11;
    
    SDL_RenderCopy(game->ren, game->inv2, &clip, &scale);
}

void invader3_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        SCALING_FACTOR * 12, SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 12, 8};
    
    scale.x = SCALING_FACTOR * game->invaders.horde[i].x;
    scale.y = SCALING_FACTOR * game->invaders.horde[i].y;
    
    clip.x = 0;
    if (game->invaders.horde[i].move_anim_frame == 1)
        clip.x = 12;
    
    SDL_RenderCopy(game->ren, game->inv3, &clip, &scale);
}

void invaders_render(struct GameData *game)
{
    static SDL_Rect const death123_clip = {0, 0, 13, 8};

    // Just aliases
    struct Invader123Instance *horde = game->invaders.horde;
    struct InvadersData *invaders = &game->invaders;

    for (int i = 0; i < HORDE_SIZE; i++)
    {
        if (horde[i].alive)
        {
            switch (horde[i].type)
            {
            case INVADER1:
                invader1_render(game, i); break;
            case INVADER2:
                invader2_render(game, i); break;
            case INVADER3:
                invader3_render(game, i); break;
            }
        }
        else if (horde[i].death_anim_time > 0) // not alive. Render death animation
        {
            SDL_Rect death123_scale = {SCALING_FACTOR*horde[i].x, SCALING_FACTOR*horde[i].y,
                SCALING_FACTOR*13, SCALING_FACTOR*8};
            switch (horde[i].type)
            {
            case INVADER1:
                death123_scale.x -= SCALING_FACTOR*2; break;
            case INVADER2:
                death123_scale.x -= SCALING_FACTOR*1; break;
            }
            SDL_RenderCopy(game->ren, game->death123, &death123_clip, &death123_scale);
        }
    }

    // special one here
}