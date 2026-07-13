#include "../include/textures.h"
#include "../include/lmath.h"
#include "../include/id.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>
#include <cstddef>
#include <map>

SDL_Texture *TEXTURES_TILEMAP;

std::map<int, vector4f_t> id_to_texture_dict;

int LoadTexturesFromFile(SDL_Renderer *renderer, const char *file)
{
    SDL_Surface *tmp_surface = SDL_LoadSurface(file);

    TEXTURES_TILEMAP = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    SDL_DestroySurface(tmp_surface);

    if (TEXTURES_TILEMAP == NULL) return -1;

    if (!SDL_SetTextureScaleMode(TEXTURES_TILEMAP, SDL_SCALEMODE_NEAREST)) return -1;

    id_to_texture_dict[1] = {0, 80, 30, 45};  // Player
    id_to_texture_dict[DIRT_BLOCK_ID] = {0,  0, 16, 16};  // Dirt block
    id_to_texture_dict[GRASS_BLOCK_ID] = {16, 0, 16, 16};  // Grass Block
    id_to_texture_dict[SKY_BLOCK_ID] = {16, 16, 16, 16};
    id_to_texture_dict[HOVER_BLOCK_ID] = {0, 16, 16, 16};  // hover

    return 0;
}

void DeleteTextures()
{
    SDL_DestroyTexture(TEXTURES_TILEMAP);
}
