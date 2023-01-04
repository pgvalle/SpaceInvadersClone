#include "Renderer.h"
#include "../Application.h"

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// ========================================================================= //
// Helpers
// ========================================================================= //

int FindInCharacterMap(char c)
{
    static const char* characterMap = APP_CHARACTER_SET;
    const char upperC = toupper(c);
    int j = 0;
    for (int i = 0; i < strlen(characterMap); i++)
    {
        if (characterMap[i] == upperC && characterMap[i] != ' ')
            return j;
        else if (characterMap[i] != ' ')
            j++;
    }

    return -1;
}

void RenderText(int x, int y, int clipY, const char* text)
{
    const int factor = app.options.scale;
    for (int i = 0; i < strlen(text); i++)
    {
        const int indexMapping = FindInCharacterMap(text[i]);
        if (indexMapping != -1) // success
        {
            SDL_Rect clip = {
                .x = indexMapping * APP_FONT_PTSIZE - 1,
                .y = clipY,
                .w = APP_FONT_PTSIZE,
                .h = APP_FONT_PTSIZE
            };
            SDL_Rect scale = {
                .x = factor * x,
                .y = factor * y,
                .w = factor * APP_FONT_PTSIZE,
                .h = factor * APP_FONT_PTSIZE
            };

            // Correcting placement of quirky characters
            // I, 1 and = are thinner.
            if (toupper(text[i]) == 'I' || text[i] == '1' || text[i] == '=')
                scale.x -= factor;
            // A is just a litle bit off to the right.
            else if (toupper(text[i]) == 'A')
                scale.x += factor;

            SDL_RenderCopy(app.renderer, app.fontTex, &clip, &scale);
        }

        x += APP_FONT_PTSIZE;
    }
}

// ========================================================================= //
// Renderer.h Implementations
// ========================================================================= //

void RenderAtlasClip(int x, int y, Clip clip)
{
    // all valid clips for atlas.png
    static const SDL_Rect clips[CLIP_COUNT] = {
        {  0,  0, 24,  8 }, // tourist
        { 24,  0, 24,  8 }, // tourist explosion

        {  0,  8, 16,  8 }, // cannon
        { 16,  8, 16,  8 }, // cannon explosion 0
        { 32,  8, 16,  8 }, // cannon explosion 1

        { 39, 18,  1,  4 }, // cannon shot
        { 36 ,24,  8,  8 }, // cannon shot explosion

        {  0, 16, 12,  8 }, // invader 0 0
        { 12, 16, 12,  8 }, // invader 0 1
        {  0, 24, 12,  8 }, // invader 1 0
        { 12, 24, 12,  8 }, // invader 1 1
        {  0, 32, 12,  8 }, // invader 2 0
        { 12, 32, 12,  8 }, // invader 2 1
        {  0, 40, 13,  8 }, // invader explosion
        
        //{}, // invader shot 1
        //{}, // invader shot 2
        //{}, // invader shot 3

        //{}, // invader shot explosion
    };

    // calculate scale rectangle according to app scaling factor
    const int factor = app.options.scale;
    const SDL_Rect scale = {
        .x = factor * x,
        .y = factor * y,
        .w = factor * clips[clip].w,
        .h = factor * clips[clip].h
    };

    SDL_RenderCopy(app.renderer, app.atlasTex, &clips[clip], &scale);
}

void RenderWhiteText(int x, int y, const char* text)
{
    const int clipY = 0;
    RenderText(x, y, clipY, text);
}

void RenderRedText(int x, int y, const char* text)
{
    const int clipY = APP_FONT_PTSIZE;
    RenderText(x, y, clipY, text);
}
