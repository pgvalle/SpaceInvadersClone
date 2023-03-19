#ifndef SCORE_H
#define SCORE_H

extern int score, high_score;

void render_score_counters();
void render_score_advances_table();

static inline
void increase_score(int amount)
{
    score += amount;
    if (score > high_score) {
        high_score = score;
    }
}


#endif // SCORE_H