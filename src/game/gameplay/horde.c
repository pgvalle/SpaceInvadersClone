#include "horde.h"
#include "game/constants.h"
#include "core/app.h"
#include "core/asset_man.h"
#include "utils/stb_ds.h"
#include <string.h>
#include <time.h>

struct horde_t horde;

void horde_init()
{
    horde.state = HORDE_STARTING;

    horde.invaders = NULL;
    horde.invaders_updated = 0;
}

void horde_remove_invader(int i)
{
    // prevent skipping any invader due to memory being moved
    if (i <= horde.invaders_updated)
        horde.invaders_updated--;
    arrdel(horde.invaders, i);
}

void horde_start_update();
void horde_move_horizontally();
void horde_move_diagonally();

void horde_update()
{
    switch (horde.state)
    {
    case HORDE_STARTING:
        horde_start_update();
        break;
    case HORDE_LOCKED: // locked. Just ignore
        break;
    case HORDE_MOVING_LEFT:
    case HORDE_MOVING_RIGHT:
        horde_move_horizontally();
        break;
    case HORDE_MOVING_DOWN_LEFT:
    case HORDE_MOVING_DOWN_RIGHT:
        horde_move_diagonally();
        break;
    }
}

void horde_start_update()
{
    if (horde.invaders_updated < HORDE_SIZE)
    {
        const int row = 4 - horde.invaders_updated / 11; // 4, 3, 2, 1, 0
        const int col = horde.invaders_updated % 11; // 0, 1, 2, ..., 10
        // clip rect for invader
        SDL_Rect clip = {12, 16, 12, 8};
        if (row > 2) clip.y = 32; // in 4th or 5th row
        else if (row > 0) clip.y = 24; // in 2nd or 3rd row

        const struct invader_t invader = {
            .clip = clip,
            .x = HORDE_X_INIT + HORDE_COL_GAP * col,
            .y = HORDE_Y_INIT + HORDE_ROW_GAP * row
        };
        arrput(horde.invaders, invader);

        horde.invaders_updated++;
    }
    else // done. Now move right
    {
        horde.state = HORDE_MOVING_RIGHT;
        horde.invaders_updated = 0;
    }
}

void horde_move_horizontally()
{
    if (arrlen(horde.invaders) == 0)
        return;
    
    const int i = horde.invaders_updated;
    // move this guy
    horde.invaders[i].x += horde.state == HORDE_MOVING_LEFT ? -2 : 2;
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0;
    // now someone got updated
    horde.invaders_updated++;
    
    // horde updated and now it's time to flip directions
    if (horde.invaders_updated == arrlen(horde.invaders))
    {
        // find out if horde should change direction
        for (int i = 0; i < arrlen(horde.invaders); i++)
        {
            const int x = horde.invaders[i].x;
            if (x <= 10 || x >= WORLD_WIDTH - 22) // should change directions
            {
                horde.state = horde.state == HORDE_MOVING_LEFT ?
                    HORDE_MOVING_DOWN_RIGHT : HORDE_MOVING_DOWN_LEFT;
                break;
            }
        }
        
        horde.invaders_updated = 0; // no one updated
    }
}

void horde_move_diagonally()
{
    if (arrlen(horde.invaders) == 0)
        return;

    const int i = horde.invaders_updated;
    horde.invaders[i].x += horde.state == HORDE_MOVING_DOWN_LEFT ? -2 : 2;
    horde.invaders[i].y += 8;
    horde.invaders[i].clip.x = horde.invaders[i].clip.x == 0 ? 12 : 0;

    horde.invaders_updated++;
    
    if (horde.invaders_updated == arrlen(horde.invaders))
    {
        horde.state = horde.state == HORDE_MOVING_DOWN_LEFT ?
            HORDE_MOVING_LEFT : HORDE_MOVING_RIGHT;
        horde.invaders_updated = 0;
    }
}

void horde_render()
{
    for (int i = 0; i < arrlen(horde.invaders); i++)
    {        
        const SDL_Rect clip = horde.invaders[i].clip;
        const SDL_Rect scale = {
            APP_SCALE * horde.invaders[i].x,
            APP_SCALE * horde.invaders[i].y,
            APP_SCALE * clip.w,
            APP_SCALE * clip.h
        };

        SDL_RenderCopy(
            app.renderer,
            asset_man_get(ASSETTYPE_TEXTURE, ATLAS_INDEX),
            &clip,
            &scale
        ); 
    }    
}