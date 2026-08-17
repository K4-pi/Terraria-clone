#pragma once

#include <SDL3/SDL_render.h>

#include <memory>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "item.h"
#include "block.h"
#include "lmath.h"
#include "player.h"

class Player;

struct Vector2Hash {
    std::size_t operator()(const vector2_t& v) const noexcept {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

static constexpr vector2_t CHUNK_SIZE = {32, 32};
static constexpr uint16_t CHUNK_RADIUS = 2;

class Chunk
{
    public:
        std::vector<Block> m_blocks;
        vector2_t m_center_position;

        Chunk();
        Chunk(std::vector<Block> chunk_blocks);
};

static constexpr float PICKUP_DISTANCE = 50.0f;

class World
{
    private:
        std::vector<Item::ItemEntity> m_world_entities;
        std::vector<Chunk*> m_active_chunks;
        std::unordered_map<vector2_t, Chunk, Vector2Hash> m_chunks;

        void UpdateActiveChunks(vector2f_t player_position);
        std::vector<Block*> GetActiveBlocks();

    public:
        World();

        Block* m_hovered_block;
        std::vector<Block*> m_active_blocks;

        void GenerateChunks(std::vector<Block> blocks);
        void GenerateWorld(const std::uint32_t seed);
        void UpdateHoveredBlock(vector2f_t mouse_position);
        void DamageBlock(int item_hardness, float item_damage);
        void PlaceBlock(int block_id);
        void DrawWorld(SDL_Renderer* renderer, vector2f_t player_position);
        void SetHoveredBlock(int id, bool collision);
        void AddEntity(int id, vector2f_t position);
        void ManageWorldEntities(SDL_Renderer* renderer, Player *player, float delta);
};
