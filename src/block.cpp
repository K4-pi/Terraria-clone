#include "../include/block.h"

#include "../include/lmath.h"

#include <string>

Block::Block(vector2f_t position, vector2f_t size, std::string name, bool collision=true)
{
    m_position = position;
    m_size = size;
    m_collision = collision;
    m_name = name;
}