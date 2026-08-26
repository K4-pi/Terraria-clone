#pragma once

#include "engine/gui.h"
#include "game/hud.h"
#include "game/inventory.h"

#include <array>

namespace FunctionalBlocks
{
    class Furnace : public GUI::GuiObject
    {
        private:
            std::array<HUD::ItemSlot, 2> m_furnace_slots;

        public:
            GUI::Button m_smelt_btn;

            Furnace(vector2f_t position, vector2f_t size);
            void Display(SDL_Renderer *renderer);
            void ResetSlots();
            void Smelt();
            void ManageSlots(Inventory* inventory);
    };
};
