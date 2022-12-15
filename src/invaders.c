#include "gamedata.h"
#include <SDL2/SDL_image.h>

#define ANIM_TIMEOUT 17

#define DEATH_ANIM_TIMEOUT 500

SDL_Rect const death123_clip = {0, 0, 13, 8};

void invaders_initialize(GameData *data)
{
    data->inv0 = IMG_LoadTexture(data->ren, "./res/img/invader0.png");
    data->inv1 = IMG_LoadTexture(data->ren, "./res/img/invader1.png");
    data->inv2 = IMG_LoadTexture(data->ren, "./res/img/invader2.png");
    data->inv3 = IMG_LoadTexture(data->ren, "./res/img/invader3.png");
    data->death123 = IMG_LoadTexture(data->ren, "./res/img/death123.png");

    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        data->invaders.instances[i].alive = true;
        data->invaders.instances[i].has_been_updated = false;
        data->invaders.instances[i].death_anim_timeout = DEATH_ANIM_TIMEOUT;
        data->invaders.instances[i].has_played_death_anim = false;
        data->invaders.instances[i].anim_frame = 1;
    }

    for (int i = 0; i < 11; i++)
    {
        // first row
        data->invaders.instances[i].type = INVADER1;
        data->invaders.instances[i].x = 1 + 15*i;
        data->invaders.instances[i].y = 0;
        // second
        data->invaders.instances[11 + i].type = INVADER2;
        data->invaders.instances[11 + i].x = 15*i;
        data->invaders.instances[11+ i].y = 15;
        // third
        data->invaders.instances[22 + i].type = INVADER2;
        data->invaders.instances[22 + i].x = 15*i;
        data->invaders.instances[22 + i].y = 30;
        // fourth
        data->invaders.instances[33 + i].type = INVADER3;
        data->invaders.instances[33 + i].x = 15*i;
        data->invaders.instances[33 + i].y = 45;
        // fifth
        data->invaders.instances[44 + i].type = INVADER3;
        data->invaders.instances[44 + i].x = 15*i;
        data->invaders.instances[44 + i].y = 60;
    }

    data->invaders.sideways_right = true;
    data->invaders.sideways_moves_count = 8;
    data->invaders.anim_timeout = ANIM_TIMEOUT;
    
}

void invaders_destroy(GameData *data)
{
    SDL_DestroyTexture(data->inv0);
    SDL_DestroyTexture(data->inv1);
    SDL_DestroyTexture(data->inv2);
    SDL_DestroyTexture(data->inv3);
}

void invaders_update(GameData *data)
{
    if (data->invaders.anim_timeout > 0)
        data->invaders.anim_timeout -= data->event_timeout;

    Uint8 const* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_RETURN])
    {
        int random = rand() % 55;
        data->invaders.instances[random].alive = false;
    }

    if (data->invaders.sideways_moves_count == 16)
    {
        for (int i = 0; i < INVADERS_COUNT; i++)
            data->invaders.instances[i].y += 7;
        
        data->invaders.sideways_moves_count = 0;
        data->invaders.sideways_right = !data->invaders.sideways_right;
    }
    else
    {
        for (int i = 0; i < INVADERS_COUNT; i++)
        {
            // ignore already updated instances
            if (data->invaders.instances[i].has_been_updated)
                continue;
            
            if (data->invaders.anim_timeout <= 0)
            {
                // move to correct side
                if (data->invaders.sideways_right)
                    data->invaders.instances[i].x += 2;
                else
                    data->invaders.instances[i].x -= 2;

                data->invaders.instances[i].has_been_updated = true;
                if (data->invaders.anim_timeout <= 0)
                    data->invaders.anim_timeout = ANIM_TIMEOUT;
            }
        }
    }

    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        if (!data->invaders.instances[i].has_been_updated)
            return;
    }

    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        data->invaders.instances[i].has_been_updated = false;
    }
    data->invaders.sideways_moves_count += 1;
}

void invader1_render(GameData *data, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 8, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 8, 8};
    
    if (data->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * data->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * data->invaders.instances[i].y;
        
        clip.x = 0;
        if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 8;
        
        SDL_RenderCopy(data->ren, data->inv1, &clip, &scale);
    }
    else if (!data->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * data->invaders.instances[i].x,
            GAME_SCALING_FACTOR * data->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(data->ren, data->death123, &death123_clip, &death123_scale);

        data->invaders.instances[i].death_anim_timeout -= data->event_timeout;
        if (data->invaders.instances[i].death_anim_timeout <= 0)
            data->invaders.instances[i].has_played_death_anim = true;
    }
}

void invader2_render(GameData *data, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 11, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 11, 8};
    
    if (data->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * data->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * data->invaders.instances[i].y;
        
        clip.x = 0;
        if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 11;
        
        SDL_RenderCopy(data->ren, data->inv2, &clip, &scale);
    }
    else if (!data->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * data->invaders.instances[i].x,
            GAME_SCALING_FACTOR * data->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(data->ren, data->death123, &death123_clip, &death123_scale);

        data->invaders.instances[i].death_anim_timeout -= data->event_timeout;
        if (data->invaders.instances[i].death_anim_timeout <= 0)
            data->invaders.instances[i].has_played_death_anim = true;
    }
}

void invader3_render(GameData *data, int i)
{
    static SDL_Rect scale = {0, 0,
        GAME_SCALING_FACTOR * 12, GAME_SCALING_FACTOR * 8};
    static SDL_Rect clip = {0, 0, 12, 8};
    
    if (data->invaders.instances[i].alive)
    {
        scale.x = GAME_SCALING_FACTOR * data->invaders.instances[i].x;
        scale.y = GAME_SCALING_FACTOR * data->invaders.instances[i].y;
        
        clip.x = 0;
        if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 12;
        
        SDL_RenderCopy(data->ren, data->inv3, &clip, &scale);
    }
    else if (!data->invaders.instances[i].has_played_death_anim)
    {
        SDL_Rect death123_scale = {GAME_SCALING_FACTOR * data->invaders.instances[i].x,
            GAME_SCALING_FACTOR * data->invaders.instances[i].y,
            GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 8};
        SDL_RenderCopy(data->ren, data->death123, &death123_clip, &death123_scale);

        data->invaders.instances[i].death_anim_timeout -= data->event_timeout;
        if (data->invaders.instances[i].death_anim_timeout <= 0)
            data->invaders.instances[i].has_played_death_anim = true;
    }
}

void invaders_render(GameData *data)
{
    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        switch (data->invaders.instances[i].type)
        {
        case INVADER1:
            invader1_render(data, i); break;
        case INVADER2:
            invader2_render(data, i); break;
        case INVADER3:
            invader3_render(data, i); break;
        }
    }
}