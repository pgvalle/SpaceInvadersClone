#include "Shot.h"
#include "app/App.h"
#include "defines.h"


void Shot::update()
{
  y += yVel;
}

void Shot::render()
{
  const SDL_Rect rect = { x, y, 1, 8 };
  SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 255);
  SDL_RenderFillRect(app->renderer, &rect);
}
