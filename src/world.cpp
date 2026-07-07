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
    m_blocks = {
        Block({50.0f, 120.0f},  {32.0f, 32.0f}, DIRT_BLOCK_ID, true, false),
        Block({114.0f - 32, 120.0f}, {32.0f, 32.0f}, DIRT_BLOCK_ID, true, false),
        Block({178.0f - 64, 120.0f}, {32.0f, 32.0f}, DIRT_BLOCK_ID, true, false),
        Block({242.0f - 96, 120.0f}, {32.0f, 32.0f}, DIRT_BLOCK_ID, true, false)
    };
}

void World::UpdateHoveredBlock(vector2f_t mouse_position)
{
    m_hovered_block = nullptr;

    for (Block &current_block : m_blocks)
    {
        bool hover_x = std::fmax(std::fmin(mouse_position.x, current_block.m_position.x + current_block.m_size.x - GameContext::camera.x), current_block.m_position.x - GameContext::camera.x) == mouse_position.x;
        bool hover_y = std::fmax(std::fmin(mouse_position.y, current_block.m_position.y + current_block.m_size.y - GameContext::camera.y), current_block.m_position.y - GameContext::camera.y) == mouse_position.y;

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
