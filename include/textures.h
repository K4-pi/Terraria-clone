#pragma once

#include "lmath.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <map>

extern SDL_Texture *TEXTURES_TILEMAP;
extern std::map<int, vector4f_t> id_to_texture_dict;

int LoadTexturesFromFile(SDL_Renderer *renderer, const char *file);
void DeleteTextures();
