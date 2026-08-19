#pragma once

#include "id.h"

#include <array>

typedef struct {
    float base_durability;
    int hardness;
    int layer;
} block_param_t;

constexpr std::array<block_param_t, 6> BLOCKS_STATS = {{
    {   0.0f, -1, 1}, // SKY_BLOCK_ID   0
    { 100.0f, 1,  2}, // DIRT_BLOCK_ID  1
    { 100.0f, 1,  2}, // GRASS_BLOCK_ID 2
    { 250.0f, 2,  2}, // STONE_BLOCK_ID 3
    { 175.0f, 1,  1}, // WOOD_BLOCK_ID 4
    { 50.0f,  0,  1}, // LEAFS_BLOCK_ID 5
}};
