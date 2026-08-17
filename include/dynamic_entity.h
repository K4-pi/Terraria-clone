#pragma once

#include "entity.h"
#include "lmath.h"
#include "block.h"

#include <vector>

class DynamicEntity : public Entity
{
    private:
        vector2f_t m_velocity;
        bool CheckCollisionX(Block *b, float delta);
        bool CheckCollisionY(Block *b, float delta);

    public:
        DynamicEntity(vector2f_t position, vector2f_t size, int id, bool collision);
        void UpdatePosition(float delta, std::vector<Block*> *blocks);
};
