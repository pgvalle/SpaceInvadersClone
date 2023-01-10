#include "asset_man.h"
#include "internal.h"
#include <string.h>

SDL_Texture* textures[ASSETS_STORAGE_SIZE];
TTF_Font* fonts[ASSETS_STORAGE_SIZE];
Mix_Music* musics[ASSETS_STORAGE_SIZE];
Mix_Chunk* chunks[ASSETS_STORAGE_SIZE];

bool asset_man_initialized = false;

void asset_man_init()
{
    // do not initialize if already asset_man_initialized
    if (asset_man_initialized)
        return;

    memset(textures, 0, ASSETS_STORAGE_SIZE * sizeof(void*));
    memset(fonts, 0, ASSETS_STORAGE_SIZE * sizeof(void*));
    memset(musics, 0, ASSETS_STORAGE_SIZE * sizeof(void*));
    memset(chunks, 0, ASSETS_STORAGE_SIZE * sizeof(void*));

    asset_man_initialized = true;
}

void asset_man_destroy()
{
    // do not try to destroy if didn't even initialize
    if (!asset_man_initialized)
        return;

    // delete all assets
    asset_man_free_all();

    asset_man_initialized = false;
}

SDL_Texture* asset_man_load_texture(uint8_t id, const char* file)
{
    if (!asset_man_initialized)
        return NULL;
 
    textures[id] = IMG_LoadTexture(app.renderer, file);
    return textures[id];
}

TTF_Font* asset_man_load_font(uint8_t id, const char* file, int ptsize)
{
    if (!asset_man_initialized)
        return NULL;

    fonts[id] = TTF_OpenFont(file, ptsize);
    return fonts[id];
}

void asset_man_store(asset_type_t type, uint8_t id, void* asset)
{
    if (!asset_man_initialized)
        return;

    if (id >= ASSETS_STORAGE_SIZE)
        return;

    switch (type)
    {
    case ASSETTYPE_TEXTURE:
        textures[id] = asset;
        break;
    case ASSETTYPE_FONT:
        fonts[id] = asset;
        break;
    case ASSETTYPE_MUSIC:
        musics[id] = asset;
        break;
    case ASSETTYPE_SOUND:
        chunks[id] = asset;
        break;
    }
}

void asset_man_free(asset_type_t type, uint8_t id)
{
    if (id >= ASSETS_STORAGE_SIZE)
        return;
    
    switch (type)
    {
    case ASSETTYPE_TEXTURE:
        if (textures[id])
        {
            SDL_DestroyTexture(textures[id]);
            textures[id] = NULL;
        }
        break;
    case ASSETTYPE_FONT:
        if (fonts[id])
        {
            TTF_CloseFont(fonts[id]);
            fonts[id] = NULL;
        }
        break;
    case ASSETTYPE_MUSIC:
        if (musics[id])
        {
            Mix_FreeMusic(musics[id]);
            musics[id] = NULL;
        }
        break;
    case ASSETTYPE_SOUND:
        if (chunks[id])
        {
            Mix_FreeChunk(chunks[id]);
            chunks[id] = NULL;
        }
        break;
    }
}

void asset_man_free_all()
{
    for (int i = 0; i < ASSETS_STORAGE_SIZE; i++)
    {
        if (textures[i])
            SDL_DestroyTexture(textures[i]);
        if (fonts[i])
            TTF_CloseFont(fonts[i]);
        if (musics[i])
            Mix_FreeMusic(musics[i]);
        if (chunks[i])
            Mix_FreeChunk(chunks[i]);

        textures[i] = NULL;
        fonts[i] = NULL;
        musics[i] = NULL;
        chunks[i] = NULL;
    }
}

void* asset_man_get(asset_type_t type, uint8_t id)
{
    if (id >= ASSETS_STORAGE_SIZE)
        return NULL;

    switch (type)
    {
    case ASSETTYPE_TEXTURE:
        return textures[id];
    case ASSETTYPE_FONT:
        return fonts[id];
    case ASSETTYPE_MUSIC:
        return musics[id];
    case ASSETTYPE_SOUND:
        return chunks[id];
    }

    return NULL;
}
