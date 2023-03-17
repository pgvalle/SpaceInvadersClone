#include "score.h"
#include "app.h"

void render_score_counters()
{
    static char score_fmt[7];
    // score
    render_text("YOUR SCORE", 10, 8, 8);
    sprintf(score_fmt, "%06d", score);
    render_text(score_fmt, 6, 24, 24);
    // high-score
    render_text("HIGH-SCORE", 10, WIDTH - 88, 8);
    sprintf(score_fmt, "%06d", high_score);
    render_text(score_fmt, 6, WIDTH - 72, 24);
}

void render_score_advances_table()
{
    render_text("*SCORE ADVANCES TABLE*", 22, 24, 128);

    static const SDL_Rect tourist = { 0,  0, 24,  8 };
    render_clip(&tourist, 60, 152);
    render_text("=? MYSTERY", 10, 80, 152);

    static const SDL_Rect invader1 = { 0, 16, 12,  8 };
    render_clip(&invader1, 66, 168);
    render_text("=30 POINTS", 10, 80, 168);

    static const SDL_Rect invader2 = { 0, 24, 12,  8 };
    render_clip(&invader2, 66, 184);
    render_text("=20 POINTS", 10, 80, 184);

    static const SDL_Rect invader3 = { 0, 32, 12,  8 };
    render_clip(&invader3, 66, 200);
    render_text("=10 POINTS", 10, 80, 200);
}

int score, high_score;