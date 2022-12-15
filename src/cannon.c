#include "gamedata.h"
#include <SDL2/SDL_image.h>

#define SHOT_COOLDOWN 1500

void cannon_initialize(GameData *data)
{
    data->cannon.tex = IMG_LoadTexture(data->ren, "res/img/cannon.png");
    SDL_assert(data->cannon.tex != NULL);

    data->cannon.clip = (SDL_Rect){0, 0, 13, 9};
    data->cannon.x = 0;
    data->cannon.y = 0;
    data->cannon.shot_cooldown = SHOT_COOLDOWN;
    data->cannon.lives = 3;
}

void cannon_destroy(GameData *data)
{
    SDL_DestroyTexture(data->cannon.tex);
}

void cannon_processEvents(GameData *data)
{
    Uint8 const* keys = SDL_GetKeyboardState(NULL);
    // movement
    if (keys[SDL_SCANCODE_LEFT])
        data->cannon.x -= 3;
    if (keys[SDL_SCANCODE_RIGHT])
        data->cannon.x += 3;

    // shot cooldown time
    if (data->cannon.shot_cooldown > 0)
        data->cannon.shot_cooldown -= data->event_timeout;
    // shot cooled down and pressed space
    if (keys[SDL_SCANCODE_SPACE] && data->cannon.shot_cooldown <= 0)
    {
        // shoot
        printf("Shooting\n");

        data->cannon.shot_cooldown = SHOT_COOLDOWN; // reset cooldown
    }
}

void cannon_render(GameData *data)
{
    SDL_Rect rect = {
        data->cannon.x,
        data->cannon.y,
        GAME_SCALING_FACTOR*data->cannon.clip.w,
        GAME_SCALING_FACTOR*data->cannon.clip.h
    };
    SDL_RenderCopy(data->ren, data->cannon.tex, &data->cannon.clip, &rect);
}