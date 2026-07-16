#include "../include/textures.h"
#include "../include/lmath.h"
#include "../include/id.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_render.h>

#include <cstddef>
#include <map>
#include <string>
#include <vector>

SDL_Texture *TEXTURES_TILEMAP;

std::map<std::string, std::vector<texture_coordinates_t>> animation_dict;
std::map<int, texture_coordinates_t> id_to_texture_dict;
std::map<int, texture_coordinates_t> id_to_gui_texture_dict;

int LoadTexturesFromFile(SDL_Renderer *renderer, const char *file)
{
    SDL_Surface *tmp_surface = SDL_LoadSurface(file);

    TEXTURES_TILEMAP = SDL_CreateTextureFromSurface(renderer, tmp_surface);

    SDL_DestroySurface(tmp_surface);

    if (TEXTURES_TILEMAP == NULL) return -1;

    if (!SDL_SetTextureScaleMode(TEXTURES_TILEMAP, SDL_SCALEMODE_NEAREST)) return -1;

    animation_dict["player_idle"] = {{0, 112, 22, 40}, {32, 112, 22, 40}};
    animation_dict["player_walk"] = {{0, 160, 22, 40}, {32, 160, 22, 40}};
    animation_dict["player_jump"] = {{0, 208, 22, 40}, { 0, 208, 22, 40}};

    // Cracks representing blocks durability
    id_to_texture_dict[CRACK_LVL_1] = { 0, 48, 16, 16};
    id_to_texture_dict[CRACK_LVL_2] = {16, 48, 16, 16};
    id_to_texture_dict[CRACK_LVL_3] = {32, 48, 16, 16};

    id_to_texture_dict[NULL_BLOCK_ID]  = {16, 16, 16, 16};  // Noting (Sky)
    id_to_texture_dict[GRASS_BLOCK_ID] = {16,  0, 16, 16};  // Grass Block
    id_to_texture_dict[DIRT_BLOCK_ID]  = { 0,  0, 16, 16};  // Dirt block
    id_to_texture_dict[STONE_BLOCK_ID] = { 0, 32, 16, 16};  // Stone
    id_to_texture_dict[HOVER_BLOCK_ID] = { 0, 16, 16, 16};  // hover

    id_to_gui_texture_dict[0] = {96, 96, 32, 32}; // item slot

    return 0;
}

void DeleteTextures()
{
    SDL_DestroyTexture(TEXTURES_TILEMAP);
}
