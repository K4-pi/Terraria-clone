#pragma once

#include <string>

#include "lmath.h"

class Block {
    public:
        vector2f_t m_position;
        vector2f_t m_size;
        RGBA_t m_sprite;
        std::string m_name;
        bool m_collision;
        bool m_hovered;

        Block(vector2f_t position, vector2f_t size, RGBA_t sprite, std::string name, bool collision, bool hovered);
};