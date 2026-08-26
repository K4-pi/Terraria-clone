#pragma once

#include "game/item.h"
#include <array>

typedef struct {
    float base_durability;
    int hardness;
    int layer;
    Item::item_type type;
} block_param_t;

constexpr std::array<block_param_t, 10> BLOCKS_STATS = {{
    {   0.0f, -1, 1, Item::BLOCK}, // SKY_BLOCK_ID   0
    { 100.0f, 1,  1, Item::BLOCK}, // DIRT_BLOCK_ID  1
    { 100.0f, 1,  1, Item::BLOCK}, // GRASS_BLOCK_ID 2
    { 250.0f, 2,  1, Item::BLOCK}, // STONE_BLOCK_ID 3
    { 175.0f, 1,  1, Item::BLOCK}, // WOOD_BLOCK_ID 4
    { 50.0f,  1,  2, Item::BLOCK}, // WOODEN_POLE_ID 5
    { 1.0f,   1,  1, Item::FUEL},  // LEAFS_BLOCK_ID 6
    { 200.f,  2,  1, Item::FUEL},  // COAL_ORE_ID 7
    { 300.0f, 2,  1, Item::ORE},   // COPPER_ORE_ID 8
    { 150.0f, 2,  2, Item::BLOCK}  // FURNACE 9
}};
