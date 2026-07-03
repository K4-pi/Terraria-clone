#pragma once

#include <string>

#include "entity.h"
#include "lmath.h"

class Block : public Entity
{
    public:
        Block(vector2f_t position, vector2f_t size, int id, bool collision=true, bool hovered=false);
};
