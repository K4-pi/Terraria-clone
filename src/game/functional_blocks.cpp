#include "game/functional_blocks.h"
#include "engine/gui.h"
#include "game/blocks_def.h"
#include "game/hud.h"
#include "game/id.h"
#include "game/inventory.h"
#include "game/item.h"

#include <SDL3/SDL_render.h>

#include <array>

namespace FunctionalBlocks
{
    Furnace::Furnace(vector2f_t position, vector2f_t size)
        : GUI::GuiObject(position, size, FURNACE_GUI_ID)
        , m_smelt_btn {GUI::Button({position.x + 4 * 32.0f, position.y + 4 * 32.0f}, {64.0f, 32.0f}, BAR_CONNECTED_SLOT_ID)}
    {
        m_furnace_slots.at(0) = HUD::ItemSlot({position.x + 32.0f, position.y + 32.0f});
        m_furnace_slots.at(1) = HUD::ItemSlot({position.x + 32.0f, position.y + 4 * 32.0f});

        m_smelt_btn.OnClick([this]() { Smelt(); });
    }

    void Furnace::Display(SDL_Renderer *renderer)
    {
        this->Draw(renderer);

        for (HUD::ItemSlot& slot : m_furnace_slots)
        {
            if (slot.IsSlotHovered()) slot.m_id = HOVERED_ITEM_SLOT_ID;
            else slot.m_id = ITEM_SLOT_ID;

            slot.Draw(renderer);
            slot.DisplayIcon(renderer);
        }

        m_smelt_btn.Draw(renderer);
    }

    void Furnace::ResetSlots()
    {
        for (HUD::ItemSlot &slot : m_furnace_slots)
        {
            slot.m_item = nullptr;
            slot.m_item_id = 0;
        }
    }

    void Furnace::Smelt()
    {
        HUD::ItemSlot *m_ore_slot  = &m_furnace_slots.at(0);
        HUD::ItemSlot *m_fuel_slot = &m_furnace_slots.at(1);

        if (m_ore_slot->m_item && m_fuel_slot->m_item)
        {
            bool resources_not_zero = m_ore_slot->m_item->capacity > 0 && m_fuel_slot->m_item->capacity > 0;
            if (!resources_not_zero) return;

            switch (m_ore_slot->m_item_id)
            {
                case COPPER_ORE_ID:
                    AddItemToInvetory(COPPER_BAR_ID, BLOCKS_STATS[COPPER_BAR_ID].type);
                    m_ore_slot->m_item->capacity--;
                    m_fuel_slot->m_item->capacity--;
                    break;

                default: break;
            }
        }
    }

    void Furnace::ManageSlots(Inventory* inventory)
    {
        HUD::ItemSlot *m_ore_slot  = &m_furnace_slots.at(0);
        HUD::ItemSlot *m_fuel_slot = &m_furnace_slots.at(1);

        if (inventory->IsSlotSelected())
        {
            HUD::ItemSlot *item_slot = inventory->m_selected_item_slot;

            if (m_fuel_slot->IsSlotHovered() && item_slot->m_item->type == Item::FUEL)
            {
                m_fuel_slot->m_item = item_slot->m_item;
                m_fuel_slot->m_item_id = item_slot->m_item_id;

                item_slot->m_item_id = 0;
                item_slot->m_item = nullptr;
            }
            else if (m_ore_slot->IsSlotHovered() && item_slot->m_item->type == Item::ORE)
            {
                m_ore_slot->m_item = item_slot->m_item;
                m_ore_slot->m_item_id = item_slot->m_item_id;

                item_slot->m_item_id = 0;
                item_slot->m_item = nullptr;
            }
        }
    }
}
