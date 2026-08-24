#pragma once

#include "lmath.h"
#include "text.h"
#include "item.h"

#include <SDL3/SDL_render.h>
#include <array>
#include <string>

namespace GUI
{
    constexpr int SLOTS_NUMBER = 5;
    constexpr float SLOT_SIZE = 64.0f;

    constexpr float ICON_SIZE = 32.0f;
    constexpr float ICON_OFFSET = ICON_SIZE * 0.5f;

    constexpr float INVETORY_SLOTS_OFFSET = 4.0f;

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
            Item::Item *m_item;
            Text::Text m_text;

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
};
