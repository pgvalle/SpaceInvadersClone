#include "gamedata.h"
#include <SDL2/SDL_image.h>

#define ANIM_TIMEOUT 50

int64_t anim_timeout = 20;

#define DEATH_ANIM_TIMEOUT 250

SDL_Rect const death123_clip = {0, 0, 13, 8};

void invaders_reset(struct GameData *game, int x, int y)
{
    for (int i = 0; i < 11; i++)
    {
        // first row
        game->invaders.instances[i].type = INVADER1;
        game->invaders.instances[i].x = x + 2 + 16*i;
        game->invaders.instances[i].y = y;
        // second
        game->invaders.instances[11 + i].type = INVADER2;
        game->invaders.instances[11 + i].x = x + 1 + 16*i;
        game->invaders.instances[11+ i].y = y + 16;
        // third
        game->invaders.instances[22 + i].type = INVADER2;
        game->invaders.instances[22 + i].x = x + 1 + 16*i;
        game->invaders.instances[22 + i].y = y + 32;
        // fourth
        game->invaders.instances[33 + i].type = INVADER3;
        game->invaders.instances[33 + i].x = x + 16*i;
        game->invaders.instances[33 + i].y = y + 48;
        // fifth
        game->invaders.instances[44 + i].type = INVADER3;
        game->invaders.instances[44 + i].x = x + 16*i;
        game->invaders.instances[44 + i].y = y + 64;
    }

    for (int i = 0; i < INVADERS_COUNT + 1; i++)
    {
        game->invaders.instances[i].has_been_updated = false;
        game->invaders.instances[i].alive = true;
        game->invaders.instances[i].death_anim_timeout = DEATH_ANIM_TIMEOUT;
        game->invaders.instances[i].has_played_death_anim = false;
        game->invaders.instances[i].anim_frame = 1;
    }

    game->invaders.sideways_right = true;
    game->invaders.sideways_moves_count = 8;
    game->invaders.anim_timeout = anim_timeout;
}

void invaders_initialize(struct GameData *game, int x, int y)
{
    game->inv0 = IMG_LoadTexture(game->ren, "./res/img/invader0.png");
    game->inv1 = IMG_LoadTexture(game->ren, "./res/img/invader1.png");
    game->inv2 = IMG_LoadTexture(game->ren, "./res/img/invader2.png");
    game->inv3 = IMG_LoadTexture(game->ren, "./res/img/invader3.png");
    game->death123 = IMG_LoadTexture(game->ren, "./res/img/death123.png");

    invaders_reset(game, x, y);
}

void invaders_processEvents(struct GameData *game)
{
    SDL_Event event = game->event;
    if (event.type == SDL_KEYDOWN)
    {
        if (event.key.keysym.sym == SDLK_RETURN)
        {
            int random = rand() % 55;
            game->invaders.instances[random].alive = false;
        }
        else if (event.key.keysym.sym == SDLK_r)
        {
            invaders_reset(game, 26, 60);
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

void invaders_update(struct GameData *game)
{
    // update animation timeout
    game->invaders.anim_timeout -= game->frametime;
    // don't animate yet
    if (game->invaders.anim_timeout > 0)
        return;
    
    // now ok
    game->invaders.anim_timeout = anim_timeout;

    for (int i = 4; i >= 0; i--)
    {
        for (int j = 0; j < 11; j++)
        {
            int k = 11 * i + j;
            // ignore updated instances
            if (game->invaders.instances[k].has_been_updated)
                continue;

            // change animation frame
            game->invaders.instances[k].anim_frame = !game->invaders.instances[k].anim_frame;
            // now instances[k] is updated
            game->invaders.instances[k].has_been_updated = true;

            // move down
            if (game->invaders.sideways_moves_count == 16)
            {
                game->invaders.instances[k].y += 8;
            }
            else // move left or right
            {
                if (game->invaders.sideways_right)
                    game->invaders.instances[k].x += 2;
                else
                    game->invaders.instances[k].x -= 2;
            }

            // if all are updated
            for (int i = 0; i < INVADERS_COUNT; i++)
            {
                if (!game->invaders.instances[i].has_been_updated)
                    return;
            }
            for (int i = 0; i < INVADERS_COUNT; i++)
                game->invaders.instances[i].has_been_updated = false;
            
            game->invaders.sideways_moves_count = (game->invaders.sideways_moves_count + 1) % 17;
            if (game->invaders.sideways_moves_count == 0)
                game->invaders.sideways_right = !game->invaders.sideways_right;
        }
    }

    
}

void invader1_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 8, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 8, 8};
    
    if (game->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * game->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * game->invaders.instances[i].y;
        
        clip.x = 0;
        if (game->invaders.instances[i].anim_frame == 1)
            clip.x = 8;
        
        SDL_RenderCopy(game->ren, game->inv1, &clip, &scale);
    }
    else if (!game->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * game->invaders.instances[i].x,
            GAME_SCALING_FACTOR * game->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(game->ren, game->death123, &death123_clip, &death123_scale);

        game->invaders.instances[i].death_anim_timeout -= game->frametime;
        if (game->invaders.instances[i].death_anim_timeout <= 0)
            game->invaders.instances[i].has_played_death_anim = true;
    }
}

void invader2_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 11, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 11, 8};
    
    if (game->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * game->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * game->invaders.instances[i].y;
        
        clip.x = 0;
        if (game->invaders.instances[i].anim_frame == 1)
            clip.x = 11;
        
        SDL_RenderCopy(game->ren, game->inv2, &clip, &scale);
    }
    else if (!game->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * game->invaders.instances[i].x,
            GAME_SCALING_FACTOR * game->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(game->ren, game->death123, &death123_clip, &death123_scale);

        game->invaders.instances[i].death_anim_timeout -= game->frametime;
        if (game->invaders.instances[i].death_anim_timeout <= 0)
            game->invaders.instances[i].has_played_death_anim = true;
    }
}

void invader3_render(struct GameData *game, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 12, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 12, 8};
    
    if (game->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * game->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * game->invaders.instances[i].y;
        
        clip.x = 0;
        if (game->invaders.instances[i].anim_frame == 1)
            clip.x = 12;
        
        SDL_RenderCopy(game->ren, game->inv3, &clip, &scale);
    }
    else if (!game->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * game->invaders.instances[i].x,
            GAME_SCALING_FACTOR * game->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(game->ren, game->death123, &death123_clip, &death123_scale);

        game->invaders.instances[i].death_anim_timeout -= game->frametime;
        if (game->invaders.instances[i].death_anim_timeout <= 0)
            game->invaders.instances[i].has_played_death_anim = true;
    }
}

void invaders_render(struct GameData *game)
{
    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        switch (game->invaders.instances[i].type)
        {
        case INVADER1:
            invader1_render(game, i); break;
        case INVADER2:
            invader2_render(game, i); break;
        case INVADER3:
            invader3_render(game, i); break;
        }
    }
}