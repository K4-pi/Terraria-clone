#include "../include/blocks_def.h"
#include "../include/block.h"
#include "../include/lmath.h"

#include <iostream>

Block::Block()
    : Entity({0.0f, 0.0f}, {0.0f, 0.0f}, 0, false)
{}

Block::Block(vector2f_t position, vector2f_t size, int id, bool collision)
    : Entity(position, size, id, collision)
{
    try
    {
       block_param_t block_stats = BLOCKS_STATS.at(id);

       m_base_durability = block_stats.base_durability;
       m_durability = block_stats.base_durability;
       m_hardness = block_stats.hardness;
    }
    catch (...)
    {
        std::cerr << "Error in Block constructor!\nNo stats of Block: " << id << std::endl;
    }
}
