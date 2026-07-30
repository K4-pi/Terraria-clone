#pragma once

#include <map>

constexpr int MAX_STACK = 99;

namespace Item
{
    typedef enum {
        BLOCK,
        TOOL,
    } item_type;

    typedef struct {
        int item_id;
        unsigned int capacity;
        item_type type;
    } Item;

    extern std::map<int, bool> is_item_stackable;

    void LoadItemTable();
}
