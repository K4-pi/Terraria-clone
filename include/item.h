#pragma once

#include <map>

constexpr int MAX_STACK = 99;

namespace Item
{
    typedef struct {
        int item_id;
        unsigned int capacity;
    } Item;

    extern std::map<int, bool> is_item_stackable;

    void LoadItemTable();
}
