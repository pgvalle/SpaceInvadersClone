#include "Bunker.h"
#include "SIC.h"

#define BUNKER_Y 192

Bunker::Bunker()
{
  static int x = 32;

  outerBounds = {x, 192, 22, 16};
  x += 46;

  memset(bits, true, 352); // enable every point

  for (int i = 0; i < 4; i++) // line
  {
    // top edges
    for (int j = 0; j < 4 - i; j++) // column
    {
      bits[22 * i + j] = bits[22 * i - j + 21] = false;
    }

    // bottom part
    const int l = 15 - i; // bottom-up
    for (int j = 5 + i; j < 18 - i; j++) // column
    {
      bits[22 * l + j] = bits[22 * l - j + 21] = false;
    }
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

void Bunker::onRender() const
{
  SDL_Point points[352] = {0};
  for (int i = 0; i < 352; i++)
  {
    if (bits[i])
    {
      const int x = outerBounds.x + i % 22;
      const int y = outerBounds.y + i / 22;
      points[i] = {x, y};
    }
  }

  SDL_SetRenderDrawColor(g->ren, 32, 255, 32, 255); // greenish
  SDL_RenderDrawPoints(g->ren, points, 352);
}
