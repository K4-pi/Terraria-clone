#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "../include/player.h"
#include "../include/block.h"
#include "../include/input.h"
#include "../include/world.h"

#include "../include/game_context.h"

#include <vector>
#include <cmath>

Player::Player(vector2f_t position, vector2f_t size, int id, float max_speed, bool collision)
    : AnimatedSprite(position, size, id, collision)
    , m_direction   { 0, 0 }
    , m_is_grounded { false }
    , m_max_speed   { max_speed }
{}

void Player::MovePlayer(float delta, std::vector<Block> b)
{
    const float acceleration = 1.10f;
    const float friction     = 350.0f;
    const float mass         = 500.0f;

    if (m_direction.x != 0)
    {
        float world_edge = GameContext::world_size.x * GameContext::STANDARD_BLOCK_SIZE - m_size.x;

        // Doesn't allow to go off screen (off the map)
        if ((m_position.x <= 0 && m_direction.x == -1) || (m_position.x >= world_edge && m_direction.x == 1))
        {
            m_velocity.x = 0.0f;
        }
        else m_velocity.x += m_direction.x * acceleration * mass * delta;
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

    m_velocity.y += 981.0f * delta; // gravity

    for (Block block : b)
    {
        if (!block.m_collision) continue;

        bool is_colliding_x = CheckCollisionX(&block, delta);
        bool is_colliding_y = CheckCollisionY(&block, delta);

        if (is_colliding_x)
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
        }

        if (is_colliding_y)
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
        }

        if (is_colliding_x && is_colliding_y) break;
    }
    m_position.x += m_velocity.x * delta; // Move X

    m_position.y += m_velocity.y * delta; // Move Y
}

void Player::HandleInput(const InputState& input_state)
{
    if (input_state.move_left && !input_state.move_right)
    {
        m_direction.x = -1;
    }
    else if (!input_state.move_left && input_state.move_right)
    {
        m_direction.x = 1;
    }
    else
    {
        m_direction.x = 0;
    }

    if (input_state.jump)
    {
        if (m_is_grounded && m_velocity.y == 0.0f)
        {
            m_velocity.y = -400.0f;
            m_is_grounded = false;
        }
    }
}

bool Player::CheckCollisionX(Block *b, float delta)
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

bool Player::CheckCollisionY(Block *b, float delta)
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

void Player::ModifyHoverBlock(World *world, float delta)
{
    vector2f_t tmp_vector = {
        world->m_hovered_block->m_position.x + GameContext::STANDARD_BLOCK_SIZE * 0.5f,
        world->m_hovered_block->m_position.y + GameContext::STANDARD_BLOCK_SIZE * 0.5f,
    };

    if (GetDistanceTo(tmp_vector) > 70.0f) return;

    if (Input::state.mouse_left_clicked)
    {
        world->DamageBlock(1, 40.0f);
    }

    if (Input::state.mouse_right_clicked)
    {
        if (CheckCollisionX(world->m_hovered_block, delta) && CheckCollisionY(world->m_hovered_block, delta)) return;

        world->PlaceBlock(2);
    }
}
