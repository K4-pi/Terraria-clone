#include "../include/world.h"
#include "../include/game_context.h"
#include "../include/id.h"

#include <cmath>

World::World()
    : m_hovered_block { nullptr }
{
    GenerateFlatWorld();
}

std::vector<Block> &World::GetBlocks()
{
    return m_blocks;
}

Block* World::GetHoveredBlock()
{
    return m_hovered_block;
}

void World::GenerateFlatWorld()
{
    m_blocks.clear();

    const int mapWidth = GameContext::world_size.x;
    const int mapHeight = GameContext::world_size.y;
    const float blockSize = 16.0f;

    m_blocks.reserve(mapWidth * mapHeight);

    const int baseGroundLevel = 156;

    for (int col = 0; col < mapWidth; ++col)
    {
        float detailWave = std::sin(col * 0.15f) * 4.0f;   // Small, frequent bumps
        float rollingWave = std::sin(col * 0.04f) * 15.0f; // Large, sweeping hills

        // Determine the actual surface row for this specific column
        int surfaceRow = baseGroundLevel + static_cast<int>(detailWave + rollingWave);

        for (int row = 0; row < mapHeight; ++row)
        {
            float xPos = col * blockSize;
            float yPos = row * blockSize;

            if (row >= surfaceRow)
            {
                int blockType = (row == surfaceRow) ? GRASS_BLOCK_ID : DIRT_BLOCK_ID;

                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {blockSize, blockSize},
                    blockType,
                    true,
                    false
                ));
            }
            else
            {
                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {blockSize, blockSize},
                    SKY_BLOCK_ID,
                    false,
                    false
                ));
            }
        }
    }
}

void World::UpdateHoveredBlock(vector2f_t mouse_position)
{
    m_hovered_block = nullptr;

    vector2f_t real_position = {
        mouse_position.x / GameContext::camera_zoom,
        mouse_position.y / GameContext::camera_zoom
    };

    for (Block &current_block : m_blocks)
    {
        bool hover_x = std::fmax(std::fmin(real_position.x, current_block.m_position.x + current_block.m_size.x - GameContext::camera.x), current_block.m_position.x - GameContext::camera.x) == real_position.x;
        bool hover_y = std::fmax(std::fmin(real_position.y, current_block.m_position.y + current_block.m_size.y - GameContext::camera.y), current_block.m_position.y - GameContext::camera.y) == real_position.y;

        current_block.m_hovered = hover_x && hover_y;

        if (current_block.m_hovered)
        {
            m_hovered_block = &current_block;
        }
    }
}

void World::SetHoveredBlock(int id, bool collision)
{
    if (m_hovered_block == nullptr) return;

    m_hovered_block->m_id = id;
    m_hovered_block->m_collision = collision;
}

// Will be changed to draw chunks
void World::DrawWorld(SDL_Renderer* renderer)
{
    for (Block &current_block : m_blocks)
    {
        current_block.Draw(renderer);
    }
}
