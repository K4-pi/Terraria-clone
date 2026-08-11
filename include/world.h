#pragma once

#include <SDL3/SDL_render.h>

#include <vector>
#include <cstdint>

#include "item.h"
#include "block.h"
#include "lmath.h"
#include "player.h"

class Player;

static constexpr vector2_t CHUNK_SIZE = {32, 32};

class Chunk
{
    private:
        std::vector<Block> m_blocks;

    public:
        vector2f_t m_center_position;

        Chunk(std::vector<Block> chunk_blocks);
};

static constexpr float PICKUP_DISTANCE = 50.0f;

class World
{
    private:
        std::vector<Item::ItemEntity> m_world_entities;
        std::vector<Chunk> m_chunks;
        std::vector<Block> m_blocks;

    public:
        World();

        Block* m_hovered_block;

        std::vector<Block>& GetBlocks();

        void GenerateChunks();
        void GenerateWorld(const std::uint32_t seed);
        void UpdateHoveredBlock(vector2f_t mouse_position);
        void DamageBlock(int item_hardness, float item_damage);
        void PlaceBlock(int block_id);
        void DrawWorld(SDL_Renderer* renderer);
        void SetHoveredBlock(int id, bool collision);
        void AddEntity(int id, vector2f_t position);
        void ManageWorldEntities(SDL_Renderer* renderer, Player *player, float delta);
};
