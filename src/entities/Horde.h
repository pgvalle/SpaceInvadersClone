#ifndef HORDE_H_INCLUDED
#define HORDE_H_INCLUDED

// 5 rows with 11 invaders each
#define HORDE_SIZE 55

#define HORDE_X_INIT 30
#define HORDE_Y_INIT 60

#define HORDE_MOVE_DELTA_INIT 20

#define INVADER_DEATH_TIMEOUT 300

struct Horde
{
    struct Invader
    {
        enum InvaderType
        {
            INVADER_0 = 0,
            INVADER_1 = 1,
            INVADER_2 = 2
        } type;
        int x, y;
        bool alive;
        int deathTimer;
        int animationClip;
    } invaders[HORDE_SIZE];

    // if invaders die, the horde should not move until all dead invaders finish
    // their death animation
    bool locked;
    bool goingRight;
    int moveCount;
    int moveTimer;
    int moveTimeout;
};

void Horde_Initialize(struct Horde* horde);
void Horde_TimeUpdate(struct Horde* horde, int delta);

void Horde_Render(const struct Horde* horde, SDL_Renderer* renderer);

#endif // HORDE_H_INCLUDED