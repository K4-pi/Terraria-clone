#include "../include/block.h"

#include "../include/lmath.h"

Block::Block(){}

Block::Block(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, bool collision, bool hovered)
    : m_position  { position }
    , m_size      { size }
    , m_sprite    { sprite }
    , m_id        { id }
    , m_collision { collision }
    , m_hovered   { hovered }
    {}