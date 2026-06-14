#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "../include/player.h"
#include "../include/block.h"

#include <vector>
#include <cmath>

static bool CheckCollisionX(player_t *p, Block *b, float delta);
static bool CheckCollisionY(player_t *p, Block *b, float delta);

void DrawPlayer(SDL_Renderer *renderer, player_t *p)
{
    SDL_FRect body = {
        .x = p->position.x,
        .y = p->position.y,
        .w = p->body.x,
        .h = p->body.y,
    };

    // Will change when .png draw will be implemented
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

    SDL_RenderFillRect(renderer, &body);
}

void MovePlayer(player_t *p, float delta, std::vector<Block> b)
{
    const float acceleration = 1.10f;
    const float friction     = 350.0f;

    if (p->direction.x != 0)
    {
        p->velocity.x += p->direction.x * acceleration * 500.0f * delta;
    }
    else 
    {
        if (p->velocity.x > 0.0)
        {
            p->velocity.x -= friction * delta;
            if (p->velocity.x < 0.0) p->velocity.x = 0.0;  // correction
        }
        else if (p->velocity.x < 0.0)
        {
            p->velocity.x += friction * delta;
            if (p->velocity.x > 0.0) p->velocity.x = 0.0;  // correction
        }
    }

    p->velocity.x = std::fmax(std::fmin(p->velocity.x, p->max_speed), -p->max_speed);

    for (Block block : b)
    {
        if (!block.m_collision) continue;

        if (CheckCollisionX(p, &block, delta))
        {
            if (p->velocity.x > 0) // Going right
            {
                p->position.x = block.m_position.x - p->body.x;
            }
            else if (p->velocity.x < 0) // Going left
            {
                p->position.x = block.m_position.x + block.m_size.x;
            }

            p->velocity.x = 0; // Zatrzymaj ruch w bok
            break;
        }
    }
    p->position.x += p->velocity.x * delta; // Wykonaj bezpieczny ruch X

    p->velocity.y += 981.0f * delta;  // gravity

    for (Block block : b)
    {
        if (!block.m_collision) continue;

        if (CheckCollisionY(p, &block, delta))
        {
            if (p->velocity.y > 0) // Falling
            {
                p->position.y = block.m_position.y - p->body.y;
                p->is_grounded = true;
            }
            else if (p->velocity.y < 0) // Jumping
            {
                p->position.y = block.m_position.y + block.m_size.y;
            }

            p->velocity.y = 0;
            break;
        }
    } 
    p->position.y += p->velocity.y * delta;
}

static bool CheckCollisionX(player_t *p, Block *b, float delta)
{
    // next x
    float next_x = p->position.x + (p->velocity.x * delta);

    bool collision_x = next_x + p->body.x > b->m_position.x &&
                       b->m_position.x + b->m_size.x > next_x;

    // current collision y
    bool collision_y = p->position.y + p->body.y > b->m_position.y &&
                       b->m_position.y + b->m_size.y > p->position.y;

    return collision_x && collision_y;
}

static bool CheckCollisionY(player_t *p, Block *b, float delta)
{
    // current collision x
    bool collision_x = p->position.x + p->body.x > b->m_position.x &&
                       b->m_position.x + b->m_size.x > p->position.x;   

    // next y
    float next_y = p->position.y + (p->velocity.y * delta);

    bool collision_y = next_y + p->body.y > b->m_position.y &&
                       b->m_position.y + b->m_size.y > next_y;
                       
    return collision_y && collision_x;
}