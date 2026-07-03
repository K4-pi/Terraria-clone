#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "../include/player.h"
#include "../include/block.h"

#include <vector>
#include <cmath>

Player::Player(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, float max_speed, bool collision, bool hovered)
    : Entity(position, size, sprite, id, collision, hovered)
    , m_direction   { 0, 0 }
    , m_is_grounded { false }
    , m_max_speed   { max_speed }
    , keys_pressed  { false }
{}

void Player::MovePlayer(float delta, std::vector<Block> b)
{
    const float acceleration = 1.10f;
    const float friction     = 350.0f;

    if (m_direction.x != 0)
    {
        m_velocity.x += m_direction.x * acceleration * 500.0f * delta;
    }
    else
    {
        if (m_velocity.x > 0.0)
        {
            m_velocity.x -= friction * delta;
            if (m_velocity.x < 0.0) m_velocity.x = 0.0;  // correction
        }
        else if (m_velocity.x < 0.0)
        {
            m_velocity.x += friction * delta;
            if (m_velocity.x > 0.0) m_velocity.x = 0.0;  // correction
        }
    }

    m_velocity.x = std::fmax(std::fmin(m_velocity.x, m_max_speed), -m_max_speed);

    for (Block block : b)
    {
        if (!block.m_collision) continue;

        if (CheckCollisionX(&block, delta))
        {
            if (m_velocity.x > 0) // Going right
            {
                m_position.x = block.m_position.x - m_size.x;
            }
            else if (m_velocity.x < 0) // Going left
            {
                m_position.x = block.m_position.x + block.m_size.x;
            }

            m_velocity.x = 0; // Zatrzymaj ruch w bok
            break;
        }
    }
    m_position.x += m_velocity.x * delta; // Wykonaj bezpieczny ruch X

    m_velocity.y += 981.0f * delta;  // gravity

    for (Block block : b)
    {
        if (!block.m_collision) continue;

        if (CheckCollisionY(&block, delta))
        {
            if (m_velocity.y > 0) // Falling
            {
                m_position.y = block.m_position.y - m_size.y;
                m_is_grounded = true;
            }
            else if (m_velocity.y < 0) // Jumping
            {
                m_position.y = block.m_position.y + block.m_size.y;
            }

            m_velocity.y = 0;
            break;
        }
    }
    m_position.y += m_velocity.y * delta;
}

void Player::HandleInput()
{
    if (keys_pressed.A && !keys_pressed.D)
    {
        m_direction.x = -1;
    }
    else if (!keys_pressed.A && keys_pressed.D)
    {
        m_direction.x = 1;
    }
    else
    {
        m_direction.x = 0;
    }

    if (keys_pressed.SPACE)
    {
        if (m_is_grounded && m_velocity.y == 0.0f)
        {
            m_velocity.y = -400.0f;
            m_is_grounded = false;
        }
    }
}
