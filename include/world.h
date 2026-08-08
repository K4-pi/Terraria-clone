#pragma once

#include <SDL3/SDL_render.h>

#include <vector>
#include <cstdint>

#include "item.h"
#include "block.h"
#include "lmath.h"
#include "player.h"

class Player;

class World
{
    private:
        std::vector<Item::ItemEntity> m_world_entities;
        std::vector<Block> m_blocks;

    public:
        World();

        Block* m_hovered_block;

        std::vector<Block>& GetBlocks();

        void GenerateWorld(const std::uint32_t seed);
        void UpdateHoveredBlock(vector2f_t mouse_position);
        void DamageBlock(int item_hardness, float item_damage);
        void PlaceBlock(int block_id);
        void DrawWorld(SDL_Renderer* renderer);
        void SetHoveredBlock(int id, bool collision);
        void AddEntity(int id, vector2f_t position);
        void ManageWorldEntities(SDL_Renderer* renderer, Player *player, float delta);
};
