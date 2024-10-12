#include "Bunker.h"
#include "SIC.h"

#define BUNKER_Y 192

Bunker::Bunker()
{
  static int x = 32;

  outerBounds = {x, 192, 22, 16};
  x += 46;

  memset(bits, 1, 352); // enable every point

  for (int i = 0; i < 4; i++) // line
  {
    // top edges
    for (int j = 0; j < 4 - i; j++) // column
      bits[22 * i + j] = bits[22 * i - j + 21] = 0;

    // bottom part
    const int l = 15 - i; // bottom-up
    for (int j = 5 + i; j < 18 - i; j++) // column
      bits[22 * l + j] = bits[22 * l - j + 21] = 0;
  }

  bits[334] = bits[347] = true; // fix 2 bottom bits
}

/*
bool Bunker::checkAndProcessHit(const SDL_Rect &hitbox)
{
  // pre-clip
  if (!SDL_HasIntersection(&hitbox, &outerBounds))
  {
    return false;
  }

  // check where
  // process collision
  return true;
}
 */

void Bunker::draw() const
{
  SDL_Point points[352] = {0};
  for (int i = 0; i < 352; i++)
  {
    SDL_Point &p = points[i];
    if (bits[i])
    {
      p = {outerBounds.x + i % 22, outerBounds.y + i / 22};
      continue;
    }

    p = {-1, -1};
  }

  SDL_SetRenderDrawColor(sic->renderer, 32, 255, 32, 255); // greenish
  SDL_RenderDrawPoints(sic->renderer, points, 352);
}
