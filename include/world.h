#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

#include "block.h"
#include "lmath.h"

class World
{
    private:
        std::vector<Block> m_blocks;

    public:
        World();

        Block* m_hovered_block;

        std::vector<Block>& GetBlocks();

        void GenerateFlatWorld();
        void UpdateHoveredBlock(vector2f_t mouse_position);
        void DamageBlock(int item_hardness, float item_damage);
        void PlaceBlock(int block_id);
        void DrawWorld(SDL_Renderer* renderer);
        void SetHoveredBlock(int id, bool collision);
};
