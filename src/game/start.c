#include "../core.h"
#include "../utils.h"
#include "defines.h"

void load_atlas();
void load_font_atlas();
void load_window_icon();

void start_state_init()
{
    app.fullscreen = false;
    // calculate scale in fullscreen
    {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(0, &mode);
        int wscale = mode.w / WORLD_WIDTH;
        int hscale = mode.h / WORLD_HEIGHT;
        app.fs_scale = wscale < hscale ? wscale : hscale;
    }
    app.scale = DEFAULT_SCALE;
    app.volume = DEFAULT_VOLUME;

    SDL_SetWindowTitle(app.window, "Space Invaders Clone");
    
    // load window icon
    load_window_icon();

    // change window size
    SDL_SetWindowSize(
        app.window,
        app.scale * WORLD_WIDTH,
        app.scale * WORLD_HEIGHT
    );

    SDL_ShowWindow(app.window);

    load_atlas();
    load_font_atlas();
}

void start_state_destroy()
{

}

void start_state_update()
{
    if (app.event.type == SDL_QUIT)
        app.should_close = true;
}

void start_state_render()
{
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_RenderClear(app.renderer);

    
}

void load_atlas()
{
    SDL_Texture* atlas = asset_man_load_texture(0, RESOURCE_DIR "/atlas.png");
    if (!atlas)
    {
        SDL_LogError(0, "res/atlas.png couldn't be opened.\n");
        SDL_Log("Game entities will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallback_surf = SDL_CreateRGBSurfaceWithFormat(
            0, 48, 48, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallback_surf, NULL, 0xffffffff);

        // create fallback texture and free surface
        atlas = SDL_CreateTextureFromSurface(app.renderer, fallback_surf);
        asset_man_store(ASSETTYPE_TEXTURE, 0, (void*)atlas);
        SDL_FreeSurface(fallback_surf);
    }
    
}

void load_font_atlas()
{
    TTF_Font* font = TTF_OpenFont(RESOURCE_DIR "/font.ttf", FONT_PTSIZE);
    if (!font)
    {
        SDL_LogError(0, "res/font.ttf couldn't be opened.\n");
        SDL_Log("Text will be rendered as white rectangles.\n");

        // Create temporary fallback surface
        SDL_Surface* fallback_surf = SDL_CreateRGBSurfaceWithFormat(
            0, 50 * FONT_PTSIZE, 2 * FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
        );

        // paint it all white
        SDL_FillRect(fallback_surf, NULL, 0xffffffff);

        // create fallback texture and free surface
        SDL_Texture* char_atlas = SDL_CreateTextureFromSurface(app.renderer, fallback_surf);
        asset_man_store(ASSETTYPE_TEXTURE, 0, (void*)char_atlas);
        SDL_FreeSurface(fallback_surf);
    }
    else
    {
        // render all character set in different surfaces separated by color
        SDL_Surface* white_chars_surf = TTF_RenderUTF8_Solid(
            font, CHARACTER_SET_MONO, (SDL_Color){ 255, 255, 255, 255 }
        );
        SDL_Surface* red_chars_surf = TTF_RenderUTF8_Solid(
            font, CHARACTER_SET_MONO, (SDL_Color){ 216, 32, 32, 255 }
        );

        // Create temporary surface to merge everything together
        SDL_Surface* chars_surf = SDL_CreateRGBSurfaceWithFormat(
            0, white_chars_surf->w, 2 * FONT_PTSIZE, 0, SDL_PIXELFORMAT_RGB888
        );

        // the surfaces to be copied have some unused gaps
        const SDL_Rect srcRect = { 0, 0,  white_chars_surf->w, FONT_PTSIZE };
        // white goes above
        SDL_Rect destRect = { 0, 0, 0, 0 };
        SDL_BlitSurface(white_chars_surf, &srcRect, chars_surf, &destRect);
        SDL_FreeSurface(white_chars_surf);
        // red goes down
        destRect.y += FONT_PTSIZE;
        SDL_BlitSurface(red_chars_surf, &srcRect, chars_surf, &destRect);
        SDL_FreeSurface(red_chars_surf);

        // finally create the texture and free the surface
        SDL_Texture* char_atlas = SDL_CreateTextureFromSurface(app.renderer, chars_surf);
        asset_man_store(ASSETTYPE_TEXTURE, 0, (void*)char_atlas);
        SDL_FreeSurface(chars_surf);

        TTF_CloseFont(font);
    }
}

void load_window_icon()
{
    SDL_RWops* ops = SDL_RWFromFile(RESOURCE_DIR "/icon.svg", "rb");
    if (ops == NULL)
    {
        SDL_LogError(0, "Could not load res/icon.svg.\n");
        SDL_Log("The window icon is undefined.\n");
    }
    else
    {
        SDL_Surface* icon_surf = IMG_LoadSVG_RW(ops);
        SDL_SetWindowIcon(app.window, icon_surf);
        SDL_RWclose(ops);
        SDL_FreeSurface(icon_surf);
    }
}
