#include "Bunker.h"
#include "app/App.h"

#include <string.h>


static int x = 32;

Bunker::Bunker()
{
  outerBounds = {x, 192, 22, 16};
  x += 46;

  // enable every point
  memset(bits, true, 352);

  bits[293] = bits[300] = false;

  // top edges
  for (int i = 3; i >= 0; i--)
  {
    for (int j = 0; j < 4 - i; j++)
    {
      bits[22 * i + j] = bits[22 * i - j + 21] = false;
    }
  }

  // dunno
  for (int i = 8; i < 14; i++)
  {
    bits[264 + i] = bits[286 + i] = false;
  }

  // dunno
  for (int i = 6; i < 16; i++)
  {
    bits[308 + i] = bits[330 + i] = false;
  }

  /*out_rect = (SDL_Rect){ x, y, 22, 16 };

        for (int p = 0; p < 352; p++)
        {
            points[p] = (SDL_Point){
                p % 22 + x, p / 22 + y
            };
        }

        points[293] = (SDL_Point){ -1, -1 };
        points[300] = (SDL_Point){ -1, -1 };

        for (int i = 3; i >= 0; i--)
        {
            for (int j = 0; j < 4 - i; j++)
            {
                points[22 * i + j] = (SDL_Point){ -1, -1 };
                points[22 * i - j + 21] = (SDL_Point){ -1, -1 };
            }
        }

        for (int i = 8; i < 14; i++)
        {
            points[264 + i] = (SDL_Point){ -1, -1 };
            points[286 + i] = (SDL_Point){ -1, -1 };
        }

        for (int i = 6; i < 16; i++)
        {
            points[308 + i] = (SDL_Point){ -1, -1 };
            points[330 + i] = (SDL_Point){ -1, -1 };
        }
    }*/
}

bool Bunker::checkAndProcessHit(const SDL_Rect &hitbox)
{
  return false;
}

void Bunker::render() const
{
  SDL_Point points[352];
  for (int i = 0; i < 352; i++)
  {
    const int x = outerBounds.x + i % 22;
    const int y = outerBounds.y + i / 22;
    if (bits[i]) points[i] = {x, y};
    else points[i] = {-1, -1};
  }

  SDL_SetRenderDrawColor(app->renderer, 32, 255, 32, 255);
  SDL_RenderDrawPoints(app->renderer, points, 352);
}
