#include "../include/entity.h"

#include "../include/block.h"
#include "../include/textures.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

Entity::Entity(vector2f_t position, vector2f_t size, int id, bool collision, bool hovered)
    : m_position         { position }
    , m_size             { size }
    , m_id               { id }
    , m_collision        { collision }
    , m_hovered          { hovered }
    , m_velocity         { 0.0f, 0.0f }
{}

void Entity::Draw(SDL_Renderer *renderer, vector2f_t position)
{
    vector4f_t texture_source = id_to_texture_dict[m_id];

    SDL_FRect src_rect = {
        .x = texture_source.x,
        .y = texture_source.y,
        .w = texture_source.w,
        .h = texture_source.h,
    };

    SDL_FRect desc_rect = {
        .x = position.x,
        .y = position.y,
        .w = m_size.x,
        .h = m_size.y,
    };

    SDL_RenderTexture(renderer, TEXTURES_TILEMAP, &src_rect, &desc_rect);
}
