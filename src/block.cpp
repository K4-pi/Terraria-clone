#include "../include/block.h"

#include "../include/lmath.h"

#include <string>

Block::Block(){};

Block::Block(vector2f_t position, vector2f_t size, RGBA_t sprite, std::string name, bool collision=true, bool hovered=false)
{
    m_position = position;
    m_size = size;
    m_sprite = sprite;
    m_name = name;
    m_collision = collision;
    m_hovered = hovered;
}