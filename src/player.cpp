#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "../include/player.h"
#include "../include/block.h"

#include <vector>
#include <cmath>

static bool CheckCollision(player_t *p, Block *b);

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
    const float acceleration = 175.0f;
    const float friction     = 25.0f;

    if (p->direction.x != 0)
    {
        p->velocity.x = p->direction.x * acceleration * delta;
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
    
    for (Block obj : b)
    {
        if (CheckCollision(p, &obj)) return;
    }

    p->position.x += p->velocity.x;  // * delta?
    p->position.y += p->velocity.y * delta;
}

static bool CheckCollision(player_t *p, Block *b)
{
    // is colliding x
    bool collision_x = p->position.x + p->body.x >= b->m_position.x &&
                  b->m_position.x + b->m_size.x >= p->position.x;

    // moves into wall x
    bool move_x = p->position.x + p->velocity.x + p->body.x >= b->m_position.x &&
                   b->m_position.x + b->m_size.x >= p->velocity.x + p->position.x;

    // is colliding y
    bool collision_y = p->position.y + p->body.y >= b->m_position.y &&
                  b->m_position.y + b->m_size.y >= p->position.y;

    // moves into wall y
    bool move_y = p->position.y + p->velocity.y + p->body.y >= b->m_position.y &&
                   b->m_position.y + b->m_size.y >= p->velocity.y + p->position.y;

    return (collision_x && move_x) && collision_y;
}