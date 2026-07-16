#pragma once

#include <SDL3/SDL_render.h>
#include <vector>

#include "block.h"
#include "animation.h"
#include "world.h"
#include "entity.h"
#include "input_state.h"
#include "lmath.h"

class Player : public AnimatedSprite
{
    protected:
        bool CheckCollisionX(Block *b, float delta);
        bool CheckCollisionY(Block *b, float delta);

    public:
        vector2f_t m_velocity;
        vector2_t m_direction;
        bool m_is_grounded;
        float m_max_speed;

        Player(vector2f_t position, vector2f_t size, int id, float max_speed, bool collision=true);
        void MovePlayer(float delta, std::vector<Block> b);
        void HandleInput(const InputState& input_state);
        void ModifyHoverBlock(World *world, float delta);
};
