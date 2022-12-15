#include "gamedata.h"
#include <SDL2/SDL_image.h>

#define SHOT_COOLDOWN 1500

void cannon_initialize(struct GameData *game, int y)
{
    game->cannon.tex = IMG_LoadTexture(game->ren, "res/img/cannon.png");
    SDL_assert(game->cannon.tex != NULL);
    game->cannon.x = 0;
    game->cannon.y = y;
    game->cannon.shot_cooldown = SHOT_COOLDOWN;
    game->cannon.lives = 3;
}

void cannon_destroy(struct GameData *game)
{
    SDL_DestroyTexture(game->cannon.tex);
}

void cannon_update(struct GameData *game)
{
    Uint8 const* keys = SDL_GetKeyboardState(NULL);
    // movement
    if (keys[SDL_SCANCODE_LEFT])
        game->cannon.x -= 1;
    if (keys[SDL_SCANCODE_RIGHT])
        game->cannon.x += 1;

    // shot cooldown time
    if (game->cannon.shot_cooldown > 0)
        game->cannon.shot_cooldown -= game->frametime;
    // shot cooled down and pressed space
    if (keys[SDL_SCANCODE_SPACE] && game->cannon.shot_cooldown <= 0)
    {
        // shoot
        printf("Shooting\n");

        game->cannon.shot_cooldown = SHOT_COOLDOWN; // reset cooldown
    }
}

void cannon_render(struct GameData *game)
{
    static SDL_Rect const clip = {0, 0, 13, 9};
    SDL_Rect scale = {GAME_SCALING_FACTOR * game->cannon.x,
        GAME_SCALING_FACTOR * game->cannon.y,
        GAME_SCALING_FACTOR * 13, GAME_SCALING_FACTOR * 9};
    SDL_RenderCopy(game->ren, game->cannon.tex, &clip, &scale);
}