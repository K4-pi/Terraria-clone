#pragma once

#include "../include/dynamic_entity.h"
#include "../include/lmath.h"

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

    class ItemEntity : public DynamicEntity
    {
        public:
            ItemEntity(vector2f_t position, vector2f_t size, int id, bool collision=true);
    };
}
