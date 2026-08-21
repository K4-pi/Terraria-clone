#include "../include/world.h"
#include "../include/player.h"
#include "../include/game_context.h"
#include "../include/id.h"
#include "../include/block.h"
#include "../include/simplex.h"
#include "../include/inventory.h"
#include "../include/blocks_def.h"

#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>

void DrawBlocks(SDL_Renderer *renderer, std::vector<Block*> *blocks);

Chunk::Chunk(std::vector<Block> chunk_blocks)
    : m_blocks { chunk_blocks }
    , m_center_position {}
{
    const Block *first_block = &m_blocks.at(0);

    m_center_position = {
        .x = int(first_block->m_position.x) + (CHUNK_SIZE.x / 2) * int(GameContext::STANDARD_BLOCK_SIZE),
        .y = int(first_block->m_position.y) + (CHUNK_SIZE.y / 2) * int(GameContext::STANDARD_BLOCK_SIZE)
    };
}

Chunk::Chunk()
    : m_blocks {}
    , m_center_position {}
{}

World::World()
    : m_world_entities {}
    , m_hovered_block { nullptr }
{}

void World::GenerateChunks(std::vector<Block> blocks)
{
    m_chunks.clear();

    const int map_width = GameContext::world_size.x;
    const int map_height = GameContext::world_size.y;

    const int chunks_x = (map_width  + CHUNK_SIZE.x - 1) / CHUNK_SIZE.x;
    const int chunks_y = (map_height + CHUNK_SIZE.y - 1) / CHUNK_SIZE.y;

    for (int cx = 0; cx < chunks_x; ++cx)
    {
        const int start_x = cx * CHUNK_SIZE.x;
        const int end_x   = std::min(start_x + CHUNK_SIZE.x, map_width); // Cap edges

        for (int cy = 0; cy < chunks_y; ++cy)
        {
            const int start_y = cy * CHUNK_SIZE.y;
            const int end_y   = std::min(start_y + CHUNK_SIZE.y, map_height); // Cap edges

            std::vector<Block> chunk_blocks;

            for (int x = start_x; x < end_x; ++x)
            {
                for (int y = start_y; y < end_y; ++y)
                {
                    const int index = x + y * map_width;

                    chunk_blocks.push_back(blocks.at(index));
                }
            }

            Chunk chunk = Chunk(chunk_blocks);
            m_chunks[{chunk.m_center_position.x, chunk.m_center_position.y}] = chunk; // Save chunk in hash map using position as key
        }
    }
}

void World::GenerateWorld(const std::uint32_t seed)
{
    std::vector<Block> blocks;

    const int map_width = GameContext::world_size.x;
    const int map_height = GameContext::world_size.y;

    blocks.reserve(map_width * map_height);

    const int base_height = round(GameContext::world_size.y * 0.5f);

    srand(seed);

    for (int col=0; col < map_width; col++)
    {
        const float scale = 0.012f;
        const float gain = 0.45f;
        const float lacunarity = 1.9f;
        const int octaves = 4;

        const float amplitude = 15.0f;

        float n1 = Simplex::GenerateValue(col, seed, octaves, scale, lacunarity, gain);
        float surface = base_height + round(n1 * amplitude);

        float n2 = Simplex::GenerateValue(col, seed, octaves, scale + 0.04f, lacunarity + 0.1f, gain + 0.2f);
        float stone_surface = base_height + 22 + round(n2 * amplitude - 10.0f);

        float xPos = col * GameContext::STANDARD_BLOCK_SIZE;

        int tree_size = 0;
        int r_num = rand() % 5;

        if (r_num == 1) // Generate tree
        {
            tree_size = (rand() % 12) + 8; // 8..12
        }

        for (int row=0; row < map_height; row++)
        {
            float yPos = row * GameContext::STANDARD_BLOCK_SIZE;

            if (row < surface)
            {
                const int TREE_INDEX = surface - tree_size;

                if (tree_size != 0 && row >= TREE_INDEX) // tree_size != 0 means that rand for tree gen was true
                {
                    if (row == TREE_INDEX)
                    {
                        blocks.push_back(Block(
                            {xPos - 16.0f, yPos - 16.0f},
                            {48.0f, 48.0f},
                            TREE_LEAFS_ID,
                            true
                        ));
                    }
                    else
                    {
                        blocks.push_back(Block(
                            {xPos, yPos},
                            {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                            WOOD_LOG_ID,
                            false
                        ));
                    }
                }
                else
                {
                    blocks.push_back(Block(
                        {xPos, yPos},
                        {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                        NULL_BLOCK_ID,
                        false
                    ));
                }
            }
            else if (row > surface && row < stone_surface)
            {
                blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    DIRT_BLOCK_ID,
                    true
                ));
            }
            else if (row == surface)
            {
                blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    GRASS_BLOCK_ID,
                    true
                ));
            }
            else
            {
                const int rand_num = rand() % 10;

                if (rand_num == 1)
                {
                    if (row >= surface + 30.0f)
                    {
                        blocks.push_back(Block(
                            {xPos, yPos},
                            {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                            COPPER_ORE_ID,
                            true
                        ));
                        continue;
                    }
                    else if (row >= surface + 15.0f)
                    {
                        blocks.push_back(Block(
                            {xPos, yPos},
                            {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                            COAL_ORE_ID,
                            true
                        ));
                        continue;
                    }
                }

                blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    STONE_BLOCK_ID,
                    true
                ));
            }
        }
    }

    GenerateChunks(blocks);
}

void World::UpdateHoveredBlock(vector2f_t mouse_position)
{
    m_hovered_block = nullptr;

    vector2f_t real_position = {
        mouse_position.x / GameContext::camera_zoom,
        mouse_position.y / GameContext::camera_zoom
    };

    for (Block *current_block : m_active_blocks)
    {
        bool hover_x = std::fmax(std::fmin(real_position.x, current_block->m_position.x + current_block->m_size.x - GameContext::camera.x), current_block->m_position.x - GameContext::camera.x) == real_position.x;
        bool hover_y = std::fmax(std::fmin(real_position.y, current_block->m_position.y + current_block->m_size.y - GameContext::camera.y), current_block->m_position.y - GameContext::camera.y) == real_position.y;

        current_block->m_hovered = hover_x && hover_y;

        if (current_block->m_hovered)
        {
            m_hovered_block = current_block;
        }
    }
}

void World::SetHoveredBlock(int id, bool collision)
{
    if (m_hovered_block == nullptr) return;

    m_hovered_block->m_id = id;
    m_hovered_block->m_collision = collision;
}

void World::DamageBlock(int item_hardness, float item_damage)
{
    if (m_hovered_block == nullptr || item_hardness < m_hovered_block->m_hardness || m_hovered_block->m_id == NULL_BLOCK_ID) return;

    m_hovered_block->m_durability -= item_damage;

    if (m_hovered_block->m_durability <= 0.0f)
    {
        AddEntity(m_hovered_block->m_id, m_hovered_block->m_position);

        *m_hovered_block = Block(
            {m_hovered_block->m_position.x, m_hovered_block->m_position.y},
            {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
            NULL_BLOCK_ID,
            false
        );
    }
}

void World::PlaceBlock(int block_id)
{
    if (m_hovered_block == nullptr || m_hovered_block->m_id != NULL_BLOCK_ID) return;

    bool block_collision = (BLOCKS_STATS[block_id].layer == 1);

    *m_hovered_block = Block(
        {m_hovered_block->m_position.x, m_hovered_block->m_position.y},
        {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
        block_id,
        block_collision
    );
}

void World::UpdateActiveChunks(vector2f_t player_position)
{
    const int CHUNK_WORLD_WIDTH = static_cast<int>(CHUNK_SIZE.x * GameContext::STANDARD_BLOCK_SIZE);
    const int CHUNK_WORLD_HEIGHT = static_cast<int>(CHUNK_SIZE.y * GameContext::STANDARD_BLOCK_SIZE);

    const int center_x = (static_cast<int>(player_position.x) / CHUNK_WORLD_WIDTH) * CHUNK_WORLD_WIDTH + CHUNK_WORLD_WIDTH / 2;
    const int center_y = (static_cast<int>(player_position.y) / CHUNK_WORLD_HEIGHT) * CHUNK_WORLD_HEIGHT + CHUNK_WORLD_HEIGHT / 2;

    m_active_chunks.clear();

    for (int cx = -CHUNK_RADIUS; cx <= CHUNK_RADIUS; ++cx)
    {
        for (int cy = -CHUNK_RADIUS; cy <= CHUNK_RADIUS; ++cy)
        {
            const vector2_t key = {
                .x = center_x + cx * CHUNK_WORLD_WIDTH,
                .y = center_y + cy * CHUNK_WORLD_HEIGHT
            };

            const auto it = m_chunks.find(key);
            if (it == m_chunks.end()) continue;

            m_active_chunks.push_back(&it->second);
        }
    }

    m_active_blocks.clear();
    m_active_blocks = GetActiveBlocks();
}

std::vector<Block*> World::GetActiveBlocks()
{
    std::vector<Block*> v;
    for (Chunk *chunk : m_active_chunks)
    {
        for (Block &b : chunk->m_blocks)
        {
            v.push_back(&b);
        }
    }

    return v;
}

void DrawBlocks(SDL_Renderer *renderer, std::vector<Block*> *blocks)
{
    // This layer draw function will be changed IF there will be more layers than 2

    std::vector<Block*> first_layer_blocks; // We will first draw second layer and then first one

    for (Block *current_block : *blocks)
    {
        if (current_block->m_layer == 1) // We add block and skip to next iteration
        {
            first_layer_blocks.push_back(current_block);
            continue;
        }

        current_block->Draw(renderer);
    }

    for (Block *current_block : first_layer_blocks)
    {
        current_block->Draw(renderer);
    }
}

void World::DrawWorld(SDL_Renderer* renderer, vector2f_t player_position)
{
    UpdateActiveChunks(player_position);

    DrawBlocks(renderer, &m_active_blocks);
}

void World::AddEntity(int id, vector2f_t position)
{
    const float BLOCK_SIZE = GameContext::STANDARD_BLOCK_SIZE * 0.5f;

    m_world_entities.push_back(Item::ItemEntity(
        { position.x + BLOCK_SIZE * 0.5f, position.y + BLOCK_SIZE * 0.5f },
        { BLOCK_SIZE, BLOCK_SIZE },
        id,
        true
    ));
}

void World::ManageWorldEntities(SDL_Renderer* renderer, Player *player, float delta)
{
    for (auto &entity : m_world_entities)
    {
        if (entity.GetDistanceTo(player->m_position) <= PICKUP_DISTANCE) // Pickup entity if player nearby
        {
            const unsigned int index = (&entity - &m_world_entities.at(0));

            m_world_entities.erase(m_world_entities.begin() + index);

            AddItemToInvetory(entity.m_id);
            continue;
        }

        entity.UpdatePosition(delta, &m_active_blocks);
        entity.Draw(renderer);
    }
}
