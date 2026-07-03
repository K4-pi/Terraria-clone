#include "../include/block.h"

#include "../include/lmath.h"

Block::Block(vector2f_t position, vector2f_t size, int id, bool collision, bool hovered)
    : Entity(position, size, id, collision, hovered)
    {}
