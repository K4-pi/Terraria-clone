#pragma once

#include <string>
#include <map>

#include "entity.h"
#include "lmath.h"

class Block : public Entity
{
    public:
        float m_base_durability;
        float m_durability;
        int m_hardness;
        int m_layer;

        Block();
        Block(vector2f_t position, vector2f_t size, int id, bool collision=true);
        void Draw(SDL_Renderer *renderer);
};
