#include "asset_man.h"
#include "app.h"
#include "../utils/stb_ds.h"

SDL_Texture** textures = NULL;
TTF_Font** fonts = NULL;
Mix_Music** songs = NULL;
Mix_Chunk** chunks = NULL;
bool asset_man_initialized = false;

void asset_man_init()
{
    // do not initialize if already asset_man_initialized
    if (asset_man_initialized)
        return;

    // adding default texture to textures
    textures = NULL;
    SDL_Surface* tmp_surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        ASSET_MAN_DEFAULT_TEXTURE_W,
        ASSET_MAN_DEFAULT_TEXTURE_H,
        0,
        SDL_PIXELFORMAT_RGB888
    );
    SDL_Texture* default_tex = SDL_CreateTextureFromSurface(
        app.renderer, tmp_surf
    );
    arrput(textures, default_tex);

    fonts = NULL;

    songs = NULL;

    chunks = NULL;

    asset_man_initialized = true;
}

void asset_man_destroy()
{
    // do not try to destroy if didn't even initialize
    if (!asset_man_initialized)
        return;

    // delete all assets (including default ones)
    
    // first get rid of the default ones
    SDL_DestroyTexture(textures[0]);
    //Mix_FreeMusic(songs[0]);
    //Mix_FreeChunk(chunks[0]);

    asset_man_free_all(); // free all the rest
    
    // delete the arrays themselves
    arrfree(textures);
    arrfree(fonts);
    arrfree(songs);
    arrfree(chunks);

    asset_man_initialized = false;
}

int asset_man_load(asset_type_t type, uint8_t id, const char* file)
{
    if (!asset_man_initialized)
        return -2;
    if (id == 0)
        return -1;

    switch (type)
    {
    case ASSETTYPE_TEXTURE:
        SDL_Texture* texture = IMG_LoadTexture(app.renderer, file);
        if (texture == NULL)
            texture = textures[0];
        if (textures[id] != NULL)
            arrins(textures, id, texture);
        break;
    }
}

void asset_man_free(asset_type_t type, uint8_t id)
{
    // safety check (asset_type[i] != asset_type[0])
    // So that the default asset isn't deleted by accident
    switch (type)
    {
    case ASSETTYPE_TEXTURE:
        if (textures[id] != textures[0])
            SDL_DestroyTexture(textures[id]);
        break;
    case ASSETTYPE_FONT:
        if (fonts[id] != fonts[0])
            TTF_CloseFont(fonts[id]);
        break;
    case ASSETTYPE_MUSIC:
        if (songs[id] != songs[0])
            Mix_FreeMusic(songs[id]);
        break;
    case ASSETTYPE_CHUNK:
        if (chunks[id] != chunks[0])
            Mix_FreeChunk(chunks[id]);
        break;
    }
}

void asset_man_free_all()
{
    for (int i = 1; i < arrlen(textures); i++)
    {
        // free all textures that aren't references to the default
        if (textures[i] && textures[i] != textures[0])
            SDL_DestroyTexture(textures[i]);
    }

    // repeat the process for all other resource types

    for (int i = 1; i < arrlen(songs); i++)
    {
        if (songs[i] && songs[i] != songs[0])
            Mix_FreeMusic(songs[i]);
    }

    for (int i = 1; i < arrlen(chunks); i++)
    {
        if (chunks[i] && chunks[i] != chunks[0])
            Mix_FreeChunk(chunks[i]);
    }

    // fonts don't have defaults
    for (int i = 0; i < arrlen(songs); i++)
    {
        if (fonts[i] && fonts[i] != fonts[0])
            TTF_CloseFont(fonts[i]);
    }
}

SDL_Texture* asset_man_get_texture(uint8_t id)
{
    if (id >= arrlen(textures))
        return NULL;
    return textures[id];
}

TTF_Font* asset_man_get_font(uint8_t id)
{
    if (id >= arrlen(fonts))
        return NULL;
    return fonts[id];
}

Mix_Music* asset_man_get_music(uint8_t id)
{
    if (id >= arrlen(songs))
        return NULL;
    return songs[id];
}

Mix_Chunk* asset_man_get_chunk(uint8_t id)
{
    if (id >= arrlen(chunks))
        return NULL;
    return chunks[id];
}
