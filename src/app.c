#include "app.h"
#include "defs.h"

void render_text(const char* text, int len, int x, int y)
{
    static const char CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789*?-<>=";
    static const int CHARS_LEN_WRAP = 6;

    for (int i = 0; i < len; i++) {
        for (int j = 0; j < sizeof(CHARS) - 1; j++) {
            if (text[i] == CHARS[j]) {
                const SDL_Rect clip = { TILE * (j % CHARS_LEN_WRAP),
                    40 + TILE * (j / CHARS_LEN_WRAP), TILE, TILE };

                render_clip(&clip, x, y);
                break;
            }
        }

        x += TILE;
    }
}

void render_score_advances_table()
{
    render_text("*SCORE ADVANCES TABLE*", 22, 24, 128);

    static const SDL_Rect tourist_r = { 0,  0, 24,  8 };
    render_clip(&tourist_r, 60, 152);
    render_text("=? MYSTERY", 10, 80, 152);

    static const SDL_Rect inv1_r = { 0, 16, 12,  8 };
    render_clip(&inv1_r, 66, 168);
    render_text("=30 POINTS", 10, 80, 168);

    static const SDL_Rect inv2_r = { 0, 24, 12,  8 };
    render_clip(&inv2_r, 66, 184);
    render_text("=20 POINTS", 10, 80, 184);

    static const SDL_Rect inv3_r = { 0, 32, 12,  8 };
    render_clip(&inv3_r, 66, 200);
    render_text("=10 POINTS", 10, 80, 200);
}

struct app_context_t ctx;
