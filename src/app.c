#include "app.h"
#include <ctype.h>

// implementing utility functions //

void render_clip(const SDL_Rect* clip, int x, int y)
{
    const SDL_Rect scale = { x, y, clip->w, clip->h };
    SDL_RenderCopy(ren, atlas, clip, &scale);
}

#define CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>="

void render_text(const char* text, int length, int x, int y)
{
    const int wrap_w = 6;
    for (int i = 0; i < length; i++)
    {
        const char c = toupper(text[i]);
        for (int j = 0; j < sizeof(CHARS) - 1; j++)
        {
            if (c == CHARS[j])
            {
                const SDL_Rect clip = {
                    TILE * (j % wrap_w), 40 + TILE * (j / wrap_w), TILE, TILE
                };
                render_clip(&clip, x, y);
                break;
            }
        }
        x += TILE;
    }
}

int point_in_rect(const SDL_Point* point, const SDL_Rect* rect)
{
    return !(point->x < rect->x || point->x >= rect->x + rect->w ||
        point->y < rect->y || point->y >= rect->y + rect->h);
}

// defining global context declared in app.h //

enum screen_t screen;
SDL_Window* win;
SDL_Renderer* ren;

int credits;
int score, hi_score;

SDL_Texture* atlas;