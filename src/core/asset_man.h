#ifndef CORE_ASSET_MAN_H
#define CORE_ASSET_MAN_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#define ASSETS_STORAGE_SIZE 16

typedef enum asset_type_t
{
    ASSETTYPE_TEXTURE,
    ASSETTYPE_FONT,
    ASSETTYPE_MUSIC,
    ASSETTYPE_SOUND  // An audio effect
} asset_type_t;

SDL_Texture* asset_man_load_texture(uint8_t id, const char* file);
TTF_Font* asset_man_load_font(uint8_t id, const char* file, int ptsize);
Mix_Music* asset_man_load_music(uint8_t id, const char* file);
Mix_Chunk* asset_man_load_sound(uint8_t id, const char* file);

// Maybe you want to procedurally generate textures or sound effects or something else.
// Note 1: Be sure to bind available ids to the new asset.
// Note 2: if you pass asset=NULL, that's valid and will be bound to the id.
void asset_man_store(asset_type_t type, uint8_t id, void* asset);

// frees an specific asset
// If you allocated some assets without sdl.
// You should free them by secure means yourself
void asset_man_free(asset_type_t type, uint8_t id);
// Deletes all assets of the given type
void asset_man_free_all_type(asset_type_t type); 
// Deletes all assets
void asset_man_free_all();

void* asset_man_get(asset_type_t type, uint8_t id);


#endif // CORE_ASSET_MAN_H
