#define CTX_IMPL
#include "core.h"


namespace ctx
{
  void load_imgs()
  {
    SDL_Surface *tmp_surface = IMG_Load(ASSETS_DIR "atlas.png");
    atlas = SDL_CreateTextureFromSurface(renderer, tmp_surface);
    SDL_FreeSurface(tmp_surface);

    tmp_surface = SDL_LoadBMP(ASSETS_DIR "icon.bmp");
    SDL_SetWindowIcon(window, tmp_surface);
    SDL_FreeSurface(tmp_surface);
  }

  void load_sfx()
  {
    // ...
  }

  void unload_sfx()
  {
    // ...
  }


  void init()
  {
    window = SDL_CreateWindow(
        "Space Invaders Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        2 * WIDTH,
        2 * HEIGHT,
        0);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetScale(renderer, 2, 2); // resolution independent rendering

    font = TTF_OpenFont(ASSETS_DIR "ps2p.ttf", 8);

    load_imgs();
  
    load_sfx();
  }

  void destroy()
  {
    unload_sfx();

    TTF_CloseFont(font);

    SDL_DestroyTexture(atlas);
  
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
  }
}
