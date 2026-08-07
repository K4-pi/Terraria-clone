#include "../include/inventory.h"
#include "../include/id.h"
#include "../include/item.h"

#include <array>

std::array<Item::Item, INVENTORY_CAPACITY> items_in_inventory = {};
std::array<Item::Item*, ITEM_BAR_CAPACITY> items_on_item_bar = {};

void UpdateInventory()
{
    Item::LoadItemTable();

    items_in_inventory[0] = Item::Item{STONE_BLOCK_ID, 32, Item::BLOCK}; // for testing
    items_in_inventory[1] = Item::Item{GRASS_BLOCK_ID, 32, Item::BLOCK};
    items_in_inventory[2] = Item::Item{DIRT_BLOCK_ID, 32, Item::BLOCK};

    items_on_item_bar[0] = &items_in_inventory[0]; // for testing
    items_on_item_bar[1] = &items_in_inventory[1];
    items_on_item_bar[2] = &items_in_inventory[2];
    items_on_item_bar[3] = &items_in_inventory[3];
    items_on_item_bar[4] = &items_in_inventory[4];
}
