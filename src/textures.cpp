#include "../include/textures.h"
#include "../include/lmath.h"

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

    id_to_texture_dict[0] = {0, 16, 16, 16};  // Test Block
    id_to_texture_dict[1] = {0, 32, 16, 32};  // Player
    id_to_texture_dict[2] = {0,  0, 16, 16};  // Dirt block
    id_to_texture_dict[6] = {0, 16, 16, 16};  // hover

    return 0;
}

void DeleteTextures()
{
    SDL_DestroyTexture(TEXTURES_TILEMAP);
}
