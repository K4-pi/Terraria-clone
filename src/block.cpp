#include "../include/blocks_def.h"
#include "../include/block.h"
#include "../include/lmath.h"
#include "../include/textures.h"
#include "../include/game_context.h"

#include <SDL3/SDL_render.h>

#include <SDL3/SDL_surface.h>
#include <iostream>

Block::Block()
    : Entity({0.0f, 0.0f}, {0.0f, 0.0f}, 0, false)
{}

Block::Block(vector2f_t position, vector2f_t size, int id, bool collision)
    : Entity(position, size, id, collision)
{
    try
    {
       block_param_t block_stats = BLOCKS_STATS.at(id);

       m_base_durability = block_stats.base_durability;
       m_durability = block_stats.base_durability;
       m_hardness = block_stats.hardness;
       m_layer = block_stats.layer;
    }
    catch (...)
    {
        std::cerr << "Error in Block constructor!\nNo stats of Block: " << id << std::endl;
    }
}

void Block::Draw(SDL_Renderer *renderer)
{
    const texture_coordinates_t texture_source = id_to_texture_dict[m_id];

    SDL_FRect src_rect = {
        .x = texture_source.x,
        .y = texture_source.y,
        .w = texture_source.w,
        .h = texture_source.h,
    };

    const vector2f_t position_on_screen = GetPositionOnScreen();

    SDL_FRect dest_rect = {
        .x = position_on_screen.x,
        .y = position_on_screen.y,
        .w = m_size.x * GameContext::camera_zoom,
        .h = m_size.y * GameContext::camera_zoom,
    };

    if (!SDL_RenderTextureRotated(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect, 0.0f, NULL, SDL_FLIP_NONE))
    {
        SDL_Log("Failed to draw Entity: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if (m_durability != m_base_durability)
    {
        texture_coordinates_t texture_source;

        if (m_durability <= m_base_durability * 0.25f)
        {
            texture_source = id_to_texture_dict[CRACK_LVL_3];
        }
        else if (m_durability <= m_base_durability * 0.5f)
        {
            texture_source = id_to_texture_dict[CRACK_LVL_2];
        }
        else if (m_durability < m_base_durability)
        {
            texture_source = id_to_texture_dict[CRACK_LVL_1];
        }
        else return; // We don't draw when block not damaged

        SDL_FRect src_rect = {
            .x = texture_source.x,
            .y = texture_source.y,
            .w = texture_source.w,
            .h = texture_source.h,
        };

        vector2f_t position_on_screen = GetPositionOnScreen();

        SDL_FRect dest_rect = {
            .x = position_on_screen.x,
            .y = position_on_screen.y,
            .w = m_size.x * GameContext::camera_zoom,
            .h = m_size.y * GameContext::camera_zoom,
        };

        if (!SDL_RenderTexture(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect))
        {
            SDL_Log("Failed to draw Entity: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }
}
