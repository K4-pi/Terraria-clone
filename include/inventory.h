#pragma once

#include "item.h"
#include "gui.h"

#include <array>

constexpr int ITEM_BAR_CAPACITY = 5;
constexpr int INVENTORY_CAPACITY = 25; // 5x5

extern std::array<Item::Item, INVENTORY_CAPACITY> items_in_inventory;
extern std::array<Item::Item*, ITEM_BAR_CAPACITY> items_on_item_bar;

void UpdateInventory();
void AddItemToInvetory(int item_id);

class Inventory : public GUI::GuiObject
{
    private:
        std::array<GUI::ItemSlot, INVENTORY_CAPACITY> m_inventory_slots;

    public:
        GUI::ItemSlot *m_hovered_item_slot;
        GUI::ItemSlot *m_selected_item_slot;

        Inventory(vector2f_t position);
        void Display(SDL_Renderer *renderer);
        void UpdateInventoryView();
        void SelectSlot();
        void MoveItemToHoveredSlot();
        void MapItemToItemBar(int bar_number);
        bool IsSlotSelected();
        void ResetSelectedSlot();
};
