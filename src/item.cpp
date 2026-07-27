#include "../include/item.h"
#include "../include/id.h"

#include <map>

namespace Item
{
    std::map<int, bool> is_item_stackable;

    void LoadItemTable()
    {
        is_item_stackable = {};

        is_item_stackable[DIRT_BLOCK_ID] = true;
        is_item_stackable[GRASS_BLOCK_ID] = true;
        is_item_stackable[STONE_BLOCK_ID] = true;
    }
}
