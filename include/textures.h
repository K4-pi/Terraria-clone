#pragma once

#include "lmath.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <map>
#include <string>
#include <vector>

extern SDL_Texture *TEXTURES_TILEMAP;
extern std::map<std::string, std::vector<texture_coordinates_t>> animation_dict;
extern std::map<int, texture_coordinates_t> id_to_texture_dict;
extern std::map<int, texture_coordinates_t> id_to_gui_texture_dict;

int LoadTexturesFromFile(SDL_Renderer *renderer, const char *file);
void DeleteTextures();
