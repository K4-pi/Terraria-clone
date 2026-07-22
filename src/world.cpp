#include "../include/world.h"
#include "../include/game_context.h"
#include "../include/id.h"
#include "../include/block.h"
#include "../include/textures.h"
#include "../include/simplex.h"

#include <SDL3/SDL_log.h>
#include <cmath>
#include <cstdint>
#include <cstdlib>

World::World()
    : m_hovered_block { nullptr }
{
    GenerateWorld();
}

std::vector<Block> &World::GetBlocks()
{
    return m_blocks;
}

// Will be changed for noise world generation
void World::GenerateWorld()
{
    m_blocks.clear();

    const int mapWidth = GameContext::world_size.x;
    const int mapHeight = GameContext::world_size.y;

    m_blocks.reserve(mapWidth * mapHeight);

    const int base_height = round(GameContext::world_size.y * 0.5f);

    const std::uint32_t seed = 964; // keep as 3 digits for now

    for (int col=0; col < GameContext::world_size.x; col++)
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

        for (int row=0; row < GameContext::world_size.y; row++)
        {
            float yPos = row * GameContext::STANDARD_BLOCK_SIZE;

            if (row < surface)
            {
                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    NULL_BLOCK_ID,
                    false
                ));
            }
            else if (row > surface && row < stone_surface)
            {
                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    DIRT_BLOCK_ID,
                    true
                ));
            }
            else if (row == surface)
            {
                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    GRASS_BLOCK_ID,
                    true
                ));
            }
            else
            {
                m_blocks.push_back(Block(
                    {xPos, yPos},
                    {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
                    STONE_BLOCK_ID,
                    true
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

void World::DamageBlock(int item_hardness, float item_damage)
{
    if (m_hovered_block == nullptr || item_hardness < m_hovered_block->m_hardness) return;

    m_hovered_block->m_durability -= item_damage;

    if (m_hovered_block->m_durability <= 0.0f)
    {
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

    *m_hovered_block = Block(
        {m_hovered_block->m_position.x, m_hovered_block->m_position.y},
        {GameContext::STANDARD_BLOCK_SIZE, GameContext::STANDARD_BLOCK_SIZE},
        block_id,
        true
    );
}

// Will be changed to draw chunks
void World::DrawWorld(SDL_Renderer* renderer)
{
    for (Block &current_block : m_blocks)
    {
        current_block.Draw(renderer);

        if (current_block.m_durability != current_block.m_base_durability)
        {
            texture_coordinates_t texture_source;

            if (current_block.m_durability <= current_block.m_base_durability * 0.25f)
            {
                texture_source = id_to_texture_dict[CRACK_LVL_3];
            }
            else if (current_block.m_durability <= current_block.m_base_durability * 0.5f)
            {
                texture_source = id_to_texture_dict[CRACK_LVL_2];
            }
            else if (current_block.m_durability < current_block.m_base_durability)
            {
                texture_source = id_to_texture_dict[CRACK_LVL_1];
            }
            else continue;

            SDL_FRect src_rect = {
                .x = texture_source.x,
                .y = texture_source.y,
                .w = texture_source.w,
                .h = texture_source.h,
            };

            vector2f_t position_on_screen = current_block.GetPositionOnScreen();

            SDL_FRect dest_rect = {
                .x = position_on_screen.x,
                .y = position_on_screen.y,
                .w = current_block.m_size.x * GameContext::camera_zoom,
                .h = current_block.m_size.y * GameContext::camera_zoom,
            };

            if (!SDL_RenderTexture(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect))
            {
                SDL_Log("Failed to draw Entity: %s", SDL_GetError());
                exit(EXIT_FAILURE);
            }
        }
    }
}
