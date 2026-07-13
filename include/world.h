#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

#include "block.h"
#include "lmath.h"

class World
{
    private:
        std::vector<Block> m_blocks;
        Block* m_hovered_block;

    public:
        World();

        std::vector<Block>& GetBlocks();
        Block* GetHoveredBlock();

        void GenerateFlatWorld();
        void UpdateHoveredBlock(vector2f_t mouse_position);
        void DrawWorld(SDL_Renderer* renderer);
        void SetHoveredBlock(int id, bool collision);
};
