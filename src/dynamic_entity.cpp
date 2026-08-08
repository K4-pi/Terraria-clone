#include "../include/dynamic_entity.h"
#include "../include/lmath.h"
#include "../include/block.h"

#include <vector>

DynamicEntity::DynamicEntity(vector2f_t position, vector2f_t size, int id, bool collision)
    : Entity(position, size, id, collision)
    , m_velocity { 0.0f, 0.0f }
{};

bool DynamicEntity::CheckCollisionX(Block *b, float delta)
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

bool DynamicEntity::CheckCollisionY(Block *b, float delta)
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

void DynamicEntity::UpdatePosition(float delta, std::vector<Block> *blocks)
{
    // const float acceleration = 1.10f;
    // const float friction     = 350.0f;
    // const float mass         = 500.0f;

    m_velocity.y += 981.0f * delta; // gravity

    for (auto &block : *blocks)
    {
        if (!block.m_collision) continue;

        if (CheckCollisionY(&block, delta))
        {
            m_velocity.y = 0.0f;
        }
    }

    m_position.y += m_velocity.y * delta; // Move Y
};