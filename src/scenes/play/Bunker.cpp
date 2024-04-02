#include "Bunker.h"
#include "app/App.h"

#include <string.h>


Bunker::Bunker(int x, int y)
{
  outerBounds = {x, y, 22, 16};

  // enable every point
  memset(points, true, 352);

  points[293] = points[300] = false;

  // top edges
  for (int i = 3; i >= 0; i--)
  {
    for (int j = 0; j < 4 - i; j++)
    {
      points[22 * i + j] = points[22 * i - j + 21] = false;
    }
  }

  // dunno
  for (int i = 8; i < 14; i++)
  {
    points[264 + i] = points[286 + i] = false;
  }

  // dunno
  for (int i = 6; i < 16; i++)
  {
    points[308 + i] = points[330 + i] = false;
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

bool checkAndProcessHit(const SDL_Rect &hitbox)
{
  return false;
}

void render() const
{
  SDL_Point points[352];
  for (int p = 0; p < 352; p++)
  {
    const int xOff = p % 22;
    const int yOff = p / 22;
    points[p] = {outerBounds.x + xOff, outerBounds.y + yOff};
  }

  SDL_RenderDrawPoints(renderer, points, 352);
}
