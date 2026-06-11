#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

#include "../include/player.h"

#include <cmath>

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

void MovePlayer(player_t *p, float delta)
{
    const float acceleration = 75.0f;
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
    
    p->position.x += p->velocity.x;  // * delta?
}
