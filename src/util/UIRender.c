#include "UIRender.h"
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
    sprintf(valueStr, "%llu", value);

    RenderText(x, y, valueStr, red);
}
