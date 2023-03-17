#ifndef UI_H
#define UI_H

extern int credit_coins;

static inline
void add_credit_coin()
{
    if (credit_coins < 99)
        credit_coins++;
}

static inline
void remove_credit_coin()
{
    if (credit_coins > 0)
        credit_coins--;
}

void process_credit_coin_events();
void render_credit_coin_counter();


#endif // UI_H
