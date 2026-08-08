#pragma once

#include "item.h"

#include <array>

constexpr int ITEM_BAR_CAPACITY = 5;
constexpr int INVENTORY_CAPACITY = 25; // 5x5

extern std::array<Item::Item, INVENTORY_CAPACITY> items_in_inventory;
extern std::array<Item::Item*, ITEM_BAR_CAPACITY> items_on_item_bar;

void UpdateInventory();
void AddItemToInvetory(int item_id);