#include "gamedata.h"
#include <SDL2/SDL_image.h>

void invaders_initialize(GameData *data)
{
    data->invaders.tex0 = IMG_LoadTexture(data->ren, "./res/img/invader0.png");
    data->invaders.tex1 = IMG_LoadTexture(data->ren, "./res/img/invader1.png");
    data->invaders.tex2 = IMG_LoadTexture(data->ren, "./res/img/invader2.png");
    data->invaders.tex3 = IMG_LoadTexture(data->ren, "./res/img/invader3.png");

    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        data->invaders.instances[i].alive = true;
        data->invaders.instances[i].has_played_death_anim = false;
        data->invaders.instances[i].anim_frame = 0;
    }

    for (int i = 0; i < 11; i++)
    {
        // first row
        data->invaders.instances[i].type = INVADER1;
        data->invaders.instances[i].x = GAME_SCALING_FACTOR*(1 + 15*i);
        data->invaders.instances[i].y = 0;
        // second
        data->invaders.instances[11 + i].type = INVADER2;
        data->invaders.instances[11 + i].x = GAME_SCALING_FACTOR*(15*i);
        data->invaders.instances[11+ i].y = GAME_SCALING_FACTOR*15;
        // third
        data->invaders.instances[22 + i].type = INVADER2;
        data->invaders.instances[22 + i].x = GAME_SCALING_FACTOR*(15*i);
        data->invaders.instances[22 + i].y = GAME_SCALING_FACTOR*30;
        // fourth
        data->invaders.instances[33 + i].type = INVADER3;
        data->invaders.instances[33 + i].x = GAME_SCALING_FACTOR*15*i;
        data->invaders.instances[33 + i].y = GAME_SCALING_FACTOR*45;
        // fifth
        data->invaders.instances[44 + i].type = INVADER3;
        data->invaders.instances[44 + i].x = GAME_SCALING_FACTOR*15*i;
        data->invaders.instances[44 + i].y = GAME_SCALING_FACTOR*60;
    }
}

void invaders_destroy(GameData *data)
{
    SDL_DestroyTexture(data->invaders.tex0);
    SDL_DestroyTexture(data->invaders.tex1);
    SDL_DestroyTexture(data->invaders.tex2);
    SDL_DestroyTexture(data->invaders.tex3);
}

void invader1_render(GameData *data, int i)
{
    static SDL_Rect rect = {0, 0,
        GAME_SCALING_FACTOR * 8, GAME_SCALING_FACTOR * 8};
    if (data->invaders.instances[i].alive)
    {
        rect.x = data->invaders.instances[i].x;
        rect.y = data->invaders.instances[i].y;
        SDL_Rect clip = {0, 0, 8, 8};
        if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 8;
        SDL_RenderCopy(data->ren, data->invaders.tex1, &clip, &rect);
    }
}

void invader2_render(GameData *data, int i)
{
    static SDL_Rect rect = {0, 0,
        GAME_SCALING_FACTOR * 11, GAME_SCALING_FACTOR * 8};
    rect.x = data->invaders.instances[i].x;
    rect.y = data->invaders.instances[i].y;
    SDL_Rect clip = {0, 0, 11, 8};
    if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 11;
    SDL_RenderCopy(data->ren, data->invaders.tex2, &clip, &rect);
}

void invader3_render(GameData *data, int i)
{
    static SDL_Rect rect = {0, 0,
        GAME_SCALING_FACTOR * 12, GAME_SCALING_FACTOR * 8};
    rect.x = data->invaders.instances[i].x;
    rect.y = data->invaders.instances[i].y;
    SDL_Rect clip = {0, 0, 12, 8};
    if (data->invaders.instances[i].anim_frame == 1)
            clip.x = 12;
    SDL_RenderCopy(data->ren, data->invaders.tex3, &clip, &rect);
}

void invaders_render(GameData *data)
{
    for (int i = 0; i < INVADERS_COUNT; i++)
    {
        if (!data->invaders.instances[i].alive)
            continue;
        
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