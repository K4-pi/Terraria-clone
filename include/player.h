#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

#include "block.h"

#include "lmath.h"

typedef struct {
    float max_speed;
    vector2f_t body;
    vector2f_t position;
    vector2f_t velocity;
    vector2_t direction;
} player_t;

void DrawPlayer(SDL_Renderer *renderer, player_t *p);
void MovePlayer(player_t *p, float delta, std::vector<Block> b);
