#include "../core.h"


void App::renderClip(int x, int y, const SDL_Rect &clip)
{
  const SDL_Rect scale = {x, y, clip.w, clip.h};
  SDL_RenderCopy(renderer, atlas, &clip, &scale);
}

void App::renderText(int x, int y, const char *text, SDL_Color color)
{
  SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, color);
  SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

  const SDL_Rect textureRect = {
      x, y, surface->w, surface->h};

  SDL_RenderCopy(renderer, texture, nullptr, &textureRect);

  SDL_FreeSurface(surface);
  SDL_DestroyTexture(texture);
}