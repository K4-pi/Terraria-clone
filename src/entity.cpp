#include "../include/entity.h"

#include "../include/block.h"

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

Entity::Entity(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, bool collision, bool hovered) 
    : m_position  { position }
    , m_size      { size }
    , m_sprite    { sprite }
    , m_id        { id }
    , m_collision { collision }
    , m_hovered   { hovered }
    , m_velocity  { 0.0f, 0.0f }
{}

void Entity::Draw(SDL_Renderer *renderer)
{
    SDL_FRect body = {
        .x = m_position.x,
        .y = m_position.y,
        .w = m_size.x,
        .h = m_size.y,
    };

    // Will change when .png draw will be implemented
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

    SDL_RenderFillRect(renderer, &body);
}

bool Entity::CheckCollisionX(Block *b, float delta)
{
    // next x
    float next_x = m_position.x + (m_velocity.x * delta);

    bool collision_x = next_x + m_size.x > b->m_position.x &&
                       b->m_position.x + b->m_size.x > next_x;

    // current collision y
    bool collision_y = m_position.y + m_size.y > b->m_position.y &&
                       b->m_position.y + b->m_size.y > m_position.y;

    return collision_x && collision_y;
}

bool Entity::CheckCollisionY(Block *b, float delta)
{
    // current collision x
    bool collision_x = m_position.x + m_size.x > b->m_position.x &&
                       b->m_position.x + b->m_size.x > m_position.x;   

    // next y
    float next_y = m_position.y + (m_velocity.y * delta);

    bool collision_y = next_y + m_size.y > b->m_position.y &&
                       b->m_position.y + b->m_size.y > next_y;
                       
    return collision_y && collision_x;
}