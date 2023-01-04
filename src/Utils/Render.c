#include "Render.h"
#include "../Application.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

SDL_Texture* atlasTex = NULL, * fontTex = NULL;

// ========================================================================= //
// Declaring helpful stuff
// ========================================================================= //

void LoadAtlasTexture();
void CreateCharsTexture();

int FindInCharacterMap(char c);
void RenderText(int x, int y, int clipY, const char* text);

// ========================================================================= //
// Renderer.h Implementations
// ========================================================================= //

void LoadTextures()
{
    LoadAtlasTexture();
    CreateCharsTexture();
}

void DestroyTextures()
{
    SDL_DestroyTexture(atlasTex);
    SDL_DestroyTexture(fontTex);
}

void RenderFilledRect(int x, int y, int w, int h, Uint32 color)
{
    const int factor = app.options.scale;
    const SDL_Rect rect = { factor * x, factor * y, factor * w, factor * h };
    const Uint8 red   = (Uint8)(color >> 24);
    const Uint8 green = (Uint8)(color >> 16);
    const Uint8 blue  = (Uint8)(color >>  8);
    const Uint8 alpha = (Uint8)color;

    if (alpha)
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(app.renderer, red, green, blue, alpha);
    SDL_RenderFillRect(app.renderer, &rect);

    if (alpha)
        SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}

void RenderClip(int x, int y, Clip clip)
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
        factor * x,
        factor * y,
        factor * clips[clip].w,
        factor * clips[clip].h
    };

    SDL_RenderCopy(app.renderer, atlasTex, &clips[clip], &scale);
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

// ========================================================================= //
// Helpful stuff implementation
// ========================================================================= //

void LoadAtlasTexture()
{
    bool entitiesTexValid = true;

    atlasTex = IMG_LoadTexture(app.renderer, APP_RESOURCE_DIR "atlas.png");
    if (atlasTex == NULL)
    {
        SDL_LogError(0, "res/atlas.png couldn't be opened.\n");
        entitiesTexValid = false;
    }
    else
    {
        int width, height;
        SDL_QueryTexture(atlasTex, NULL, NULL, &width, &height);
        if (width < 48 || height < 48)
        {
            SDL_LogError(0, "res/atlas.png is %dx%d pixels.\n", width, height);
            fprintf(stderr, "It should be at least 48x48.\n");

            entitiesTexValid = false;
            SDL_DestroyTexture(atlasTex);
            atlasTex = NULL;
        }
    }

    if (!entitiesTexValid)
    {
        SDL_Log("Game entities will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallbackSurf = SDL_CreateRGBSurfaceWithFormat(
            0, 48, 48, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallbackSurf, NULL, 0xffffffff);

        // create fallback texture and free surface
        atlasTex = SDL_CreateTextureFromSurface(app.renderer, fallbackSurf);
        SDL_FreeSurface(fallbackSurf);
    }
}

void CreateCharsTexture()
{
    TTF_Font* font = TTF_OpenFont(APP_RESOURCE_DIR "font.ttf", APP_FONT_PTSIZE);
    if (font == NULL)
    {
        SDL_LogError(0, "res/font.ttf couldn't be opened.\n");
        SDL_Log("Text will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallbackSurf = SDL_CreateRGBSurfaceWithFormat(
            0, 50 * APP_FONT_PTSIZE, 2 * APP_FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallbackSurf, NULL, 0xffffffff);

        // create fallback texture and free surface
        fontTex = SDL_CreateTextureFromSurface(app.renderer, fallbackSurf);
        SDL_FreeSurface(fallbackSurf);
    }
    else
    {
        // render all character set in different surfaces separated by color
        SDL_Surface* whiteCharsSurf = TTF_RenderUTF8_Solid(
            font, APP_CHARACTER_SET, (SDL_Color){ 255, 255, 255, 255 }
        );
        SDL_Surface* redCharsSurf = TTF_RenderUTF8_Solid(
            font, APP_CHARACTER_SET, (SDL_Color){ 216, 32, 32, 255 }
        );

        // Create temporary surface to merge everything together
        SDL_Surface* allCharsSurf = SDL_CreateRGBSurfaceWithFormat(
            0, whiteCharsSurf->w, 2 * APP_FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
        );

        // the surfaces to be copied have some unused gaps
        const SDL_Rect srcRect = { 0, 0,  whiteCharsSurf->w, APP_FONT_PTSIZE };
        // white goes above
        SDL_Rect destRect = { 0, 0, 0, 0 };
        SDL_BlitSurface(whiteCharsSurf, &srcRect, allCharsSurf, &destRect);
        SDL_FreeSurface(whiteCharsSurf);
        // red goes down
        destRect.y += APP_FONT_PTSIZE;
        SDL_BlitSurface(redCharsSurf, &srcRect, allCharsSurf, &destRect);
        SDL_FreeSurface(redCharsSurf);

        // finally create the texture and free the surface
        fontTex = SDL_CreateTextureFromSurface(app.renderer, allCharsSurf);
        SDL_FreeSurface(allCharsSurf);

        TTF_CloseFont(font);
    }
}

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
                indexMapping * APP_FONT_PTSIZE - 1,
                clipY,
                APP_FONT_PTSIZE,
                APP_FONT_PTSIZE
            };
            SDL_Rect scale = {
                factor * x,
                factor * y,
                factor * APP_FONT_PTSIZE,
                factor * APP_FONT_PTSIZE
            };

            // Correcting placement of quirky characters
            // I, 1 and = are thinner.
            if (toupper(text[i]) == 'I' || text[i] == '1' || text[i] == '=')
                scale.x -= factor;
            // A is just a litle bit off to the right.
            else if (toupper(text[i]) == 'A')
                scale.x += factor;

            SDL_RenderCopy(app.renderer, fontTex, &clip, &scale);
        }

        x += APP_FONT_PTSIZE;
    }
}