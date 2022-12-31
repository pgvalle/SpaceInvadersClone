#include "Renderer.h"
#include "../Application.h"

#include <stdio.h>
#include <ctype.h>

// ========================================================================= //
// Helper Functions
// ========================================================================= //

int FindInCharacterMap(char c)
{
    static const char* characterMap = APP_CHARACTER_SET;
    const char upperC = toupper(c);
    int j = 0;
    for (int i = 0; i < strlen(characterMap); i++)
    {
        if (characterMap[i] == upperC)
            return j;
        if (characterMap[i] != ' ')
            j++;
    }

    return -1;
}


// ========================================================================= //
// Renderer.h Implementations
// ========================================================================= //

void RenderEntity(int x, int y, enum EntityTex tex)
{
    // all valid clips in atlas.png
    static const SDL_Rect rectMap[ENTITYTEX_COUNT] =
    {
        {  0,  0, 24,  8 }, // tourist
        { 24,  0, 24,  8 }, // tourist dead

        {  0,  8, 16,  8 }, // cannon
        { 16,  8, 16,  8 }, // cannon dead 0
        { 32,  8, 16,  8 }, // cannon dead 1

        {  0, 16, 12,  8 }, // invader 0 0
        { 12, 16, 12,  8 }, // invader 0 1
        {  0, 24, 12,  8 }, // invader 1 0
        { 12, 24, 12,  8 }, // invader 1 1
        {  0, 32, 12,  8 }, // invader 2 0
        { 12, 32, 12,  8 }, // invader 2 1
        {  0, 40, 13,  8 }, // invader dead
    };

    // calculate scale rectangle according to app scaling factor
    const int scaleFactor = app.options.scale;
    const SDL_Rect scale = {
        scaleFactor * x,
        scaleFactor * y,
        scaleFactor * rectMap[tex].w,
        scaleFactor * rectMap[tex].h
    };

    SDL_RenderCopy(app.renderer, app.entitiesTex, &rectMap[tex], &scale);
}

void RenderText(int x, int y, const char* text, bool red)
{
    const int scaleFactor = app.options.scale;
    const int textLen = strlen(text);
    const int clipY = red ? APP_FONT_PTSIZE : 0;
    for (int i = 0; i < textLen; i++)
    {
        const int indexMapping = FindInCharacterMap(text[i]);
        if (indexMapping != -1) // success
        {
            SDL_Rect clipRect = {
                indexMapping * APP_FONT_PTSIZE,
                clipY,
                APP_FONT_PTSIZE,
                APP_FONT_PTSIZE
            };
            SDL_Rect scaleRect = {
                scaleFactor * x,
                scaleFactor * y,
                scaleFactor * APP_FONT_PTSIZE,
                scaleFactor * APP_FONT_PTSIZE
            };

            // I and 1 are thinner. This is to correct their placement
            if (toupper(text[i]) == 'I' || text[i] == '1')
                scaleRect.x -= scaleFactor;

            SDL_RenderCopy(app.renderer, app.charsTex, &clipRect, &scaleRect);
        }

        x += APP_FONT_PTSIZE;
    }
}

void RenderUnsignedInt(int x, int y, uint64_t value, bool red)
{
    // convert value to a string first
    char valueStr[22] = "\0";
    sprintf_s(valueStr, 22, "%llu", value);

    RenderText(x, y, valueStr, red);
}
