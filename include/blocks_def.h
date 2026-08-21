#pragma once

#include "id.h"

#include <array>

typedef struct {
    float base_durability;
    int hardness;
    int layer;
} block_param_t;

constexpr std::array<block_param_t, 9> BLOCKS_STATS = {{
    {   0.0f, -1, 1}, // SKY_BLOCK_ID   0
    { 100.0f, 1,  1}, // DIRT_BLOCK_ID  1
    { 100.0f, 1,  1}, // GRASS_BLOCK_ID 2
    { 250.0f, 2,  1}, // STONE_BLOCK_ID 3
    { 175.0f, 1,  1}, // WOOD_BLOCK_ID 4
    { 50.0f,  1,  2}, // WOODEN_POLE_ID 5
    { 1.0f,   1,  1}, // LEAFS_BLOCK_ID 6
    { 200.f,  2,  1}, // COAL_ORE_ID 7
    { 300.0f, 2,  1}  // COPPER_ORE_ID 8
}};
