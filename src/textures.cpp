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

    if (tmp_surface == NULL) return -1;

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

    id_to_texture_dict[HOVER_BLOCK_ID] = { 0, 16, 16, 16};  // hover

    id_to_texture_dict[NULL_BLOCK_ID]  = {0, 16, 16, 16};  // Nothing (Sky)
    id_to_texture_dict[GRASS_BLOCK_ID] = {16,  0, 16, 16};  // Grass Block
    id_to_texture_dict[DIRT_BLOCK_ID]  = { 0,  0, 16, 16};  // Dirt block
    id_to_texture_dict[STONE_BLOCK_ID] = { 0, 32, 16, 16};  // Stone
    id_to_texture_dict[WOOD_BLOCK_ID]  = {32,  0, 16, 16};
    id_to_texture_dict[WOOD_LOG_ID]    = {16, 32, 16, 16};
    id_to_texture_dict[TREE_LEAFS_ID]  = {208, 0, 48, 48};
    id_to_texture_dict[COAL_ORE_ID]    = {16, 16, 16, 16};
    id_to_texture_dict[COPPER_ORE_ID]  = {32, 16, 16, 16};

    id_to_gui_texture_dict[ITEM_SLOT_ID]          = {64, 112, 32, 32}; // item slot
    id_to_gui_texture_dict[SELECTED_ITEM_SLOT_ID] = {96, 112, 32, 32}; // selected item slot
    id_to_gui_texture_dict[HOVERED_ITEM_SLOT_ID] = {128, 112, 32, 32}; // hovered item slot

    // Hearts sprite
    id_to_gui_texture_dict[HEART_LVL_1_ID] = {96, 80, 32, 32};
    id_to_gui_texture_dict[HEART_LVL_2_ID] = {64, 80, 32, 32};
    id_to_gui_texture_dict[HEART_LVL_3_ID] = {32, 80, 32, 32};
    id_to_gui_texture_dict[HEART_LVL_4_ID] = { 0, 80, 32, 32};

    return 0;
}

void DeleteTextures()
{
    SDL_DestroyTexture(TEXTURES_TILEMAP);
}
