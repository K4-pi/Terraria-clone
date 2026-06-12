#pragma once

#include <string>

#include "lmath.h"

class Block {
    public:
        vector2f_t m_position;
        vector2f_t m_size;
        std::string m_name;
        bool m_collision;

        Block(vector2f_t position, vector2f_t size, std::string name, bool collision);
};