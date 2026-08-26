#pragma once

#include "game/item.h"
#include "game/hud.h"

#include <array>

constexpr int ITEM_BAR_CAPACITY = 5;
constexpr int INVENTORY_CAPACITY = 25; // 5x5

extern std::array<Item::Item, INVENTORY_CAPACITY> items_in_inventory;
extern std::array<Item::Item*, ITEM_BAR_CAPACITY> items_on_item_bar;

void UpdateInventory();
void AddItemToInvetory(int item_id, Item::item_type item_type, unsigned int capacity=1);

class Inventory : public GUI::GuiObject
{
    private:
        std::array<HUD::ItemSlot, INVENTORY_CAPACITY> m_inventory_slots;

    public:
        HUD::ItemSlot *m_hovered_item_slot;
        HUD::ItemSlot *m_selected_item_slot;

        Inventory(vector2f_t position);
        void Display(SDL_Renderer *renderer);
        void UpdateInventoryView();
        void SelectSlot();
        void MoveItemToHoveredSlot();
        void MapItemToItemBar(int bar_number);
        bool IsSlotSelected();
        void ResetSelectedSlot();
};
