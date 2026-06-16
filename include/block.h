#pragma once

#include <string>

#include "lmath.h"

class Block {
    public:
        vector2f_t m_position;
        vector2f_t m_size;
        RGBA_t m_sprite;
        int m_id;
        bool m_collision;
        bool m_hovered;

        Block();
        Block(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, bool collision=true, bool hovered=false);
};