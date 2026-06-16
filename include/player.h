#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

#include "block.h"

#include "entity.h"
#include "lmath.h"

class Player : public Entity 
{
    public:
        vector2_t m_direction;
        bool m_is_grounded;
        float m_max_speed;

        struct {
            bool A, D, SPACE;
        } keys_pressed;

        Player(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, float max_speed, bool collision=true, bool hovered=false);
        void MovePlayer(float delta, std::vector<Block> b);
        void HandleInput();
};
