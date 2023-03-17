#ifndef SCORE_H
#define SCORE_H

extern int score, high_score;

static inline
void increase_score(int amount)
{
    score += amount;
    if (score > high_score) {
        high_score = score;
    }
}

void render_score_counters();
void render_score_advances_table();


#endif // SCORE_H