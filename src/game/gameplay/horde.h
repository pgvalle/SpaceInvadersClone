#ifndef HORDE_H
#define HORDE_H

#include <stdbool.h>
#include <SDL_rect.h>

struct invader_t
{
    SDL_Rect clip;
    int x, y;
};

#define HORDE_X_INIT 26
#define HORDE_Y_INIT 72

#define HORDE_ROW_GAP 16
#define HORDE_COL_GAP 16

#define HORDE_SIZE 55

#define HORDE_MOVE_TIMEOUT_INIT 1024

enum horde_state_t
{
    HORDE_STARTING, // start animation
    HORDE_LOCKED,
    HORDE_MOVING_LEFT,
    HORDE_MOVING_DOWN_LEFT,
    HORDE_MOVING_RIGHT,
    HORDE_MOVING_DOWN_RIGHT
};

extern struct horde_t
{
    enum horde_state_t state;

    struct invader_t* invaders;
    // One invader is updated each frame.
    // horde is up-to-date only when all invaders are up-to-date.
    int invaders_updated;
} horde;

void horde_init();

void horde_remove_invader(int i);

void horde_update();
void horde_render();

#endif // HORDE_H
