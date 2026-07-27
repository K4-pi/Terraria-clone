#pragma once

#include "lmath.h"
#include "inventory.h"

#include <SDL3/SDL_render.h>
#include <array>
#include <string>

static constexpr int SLOTS_NUMBER = 5;

namespace GUI
{
    class GuiObject
    {
        public:
            vector2f_t m_position;
            vector2f_t m_size;
            int m_id;

            GuiObject();
            GuiObject(vector2f_t position, vector2f_t size, int id);
            void Draw(SDL_Renderer *renderer);
    };

    class ItemSlot : public GuiObject
    {
        public:
            int m_item_id;

            ItemSlot();
            ItemSlot(vector2f_t position);
            void DisplayIcon(SDL_Renderer *renderer);
            bool IsSlotHovered();
    };

    class ItemBar : public GuiObject
    {
        private:
            GUI::ItemSlot m_item_slots[SLOTS_NUMBER];

        public:
            GUI::ItemSlot *m_selected_slot;

            ItemBar(vector2f_t position);
            void Display(SDL_Renderer *renderer);
            void UpdateItemBar();
            void SelectSlot(int slot_number);
    };

    class Inventory : public GuiObject
    {
        private:
            std::array<GUI::ItemSlot, INVENTORY_CAPACITY> m_inventory_slots;

        public:
            ItemSlot *m_hovered_item_slot;
            ItemSlot *m_selected_item_slot;

            Inventory(vector2f_t position);
            void Display(SDL_Renderer *renderer);
            void UpdateInventoryView();
            void SelectSlot();
            void MoveItemToHoveredSlot();
            bool IsSlotSelected();
            void ResetSelectedSlot();
    };
};
