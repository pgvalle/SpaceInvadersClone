#include "app.h"
#include <ctype.h>

// implementing utility functions //

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

// defining global context declared in app.h //

enum screen_t screen;
SDL_Window* win;
SDL_Renderer* ren;

int credits;
int score, hi_score;

SDL_Texture* atlas;
