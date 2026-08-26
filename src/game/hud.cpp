#include "game/hud.h"
#include "engine/textures.h"
#include "game/id.h"
#include "game/game_context.h"
#include "game/inventory.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include <cmath>
#include <cstdlib>
#include <string>

namespace HUD
{
    ItemSlot::ItemSlot()
        : GuiObject({0.0f, 0.0f}, {SLOT_SIZE, SLOT_SIZE}, ITEM_SLOT_ID)
        , m_item_id {}
        , m_item { nullptr }
        , m_text { Text::Text({0.0f, 0.0f}) }
    {}

    ItemSlot::ItemSlot(vector2f_t position)
        : GuiObject(position, {SLOT_SIZE, SLOT_SIZE}, ITEM_SLOT_ID)
        , m_item_id {}
        , m_item { nullptr }
        , m_text { Text::Text(position) }
    {}

    bool ItemSlot::IsSlotHovered()
    {
        float x = std::fmin(std::fmax(m_position.x + INVETORY_SLOTS_OFFSET, GameContext::mouse_position.x), m_position.x + m_size.x - INVETORY_SLOTS_OFFSET);
        float y = std::fmin(std::fmax(m_position.y + INVETORY_SLOTS_OFFSET, GameContext::mouse_position.y), m_position.y + m_size.y - INVETORY_SLOTS_OFFSET);

        return (x == GameContext::mouse_position.x && y == GameContext::mouse_position.y) ? true : false;
    }

    void ItemSlot::DisplayIcon(SDL_Renderer *renderer)
    {
        texture_coordinates_t texture_source = id_to_texture_dict[m_item_id];

        SDL_FRect src_rect = {
            .x = texture_source.x,
            .y = texture_source.y,
            .w = texture_source.w,
            .h = texture_source.h,
        };

        SDL_FRect dest_rect = {
            .x = m_position.x + ICON_OFFSET,
            .y = m_position.y + ICON_OFFSET,
            .w = ICON_SIZE,
            .h = ICON_SIZE,
        };

        if (!SDL_RenderTexture(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect))
        {
            SDL_Log("Failed to draw Entity: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        if (m_item != nullptr && m_item->capacity > 0)
        {
            m_text.Draw(renderer, std::to_string(m_item->capacity));
        }
    }

    ItemBar::ItemBar(vector2f_t position)
        : GuiObject(position, {0, 0}, ITEM_SLOT_ID)
        , m_item_slots {
            HUD::ItemSlot({position.x + 0 * SLOT_SIZE, position.y + 16.0f}),
            HUD::ItemSlot({position.x + 1 * SLOT_SIZE, position.y + 16.0f}),
            HUD::ItemSlot({position.x + 2 * SLOT_SIZE, position.y + 16.0f}),
            HUD::ItemSlot({position.x + 3 * SLOT_SIZE, position.y + 16.0f}),
            HUD::ItemSlot({position.x + 4 * SLOT_SIZE, position.y + 16.0f})
        }
        , m_selected_slot { &m_item_slots[0] }
    {
        UpdateItemBar();

        GameContext::selected_item_slot = m_selected_slot;
        m_selected_slot->m_id = SELECTED_ITEM_SLOT_ID;
    }

    void ItemBar::UpdateItemBar()
    {
        for (int i=0; i < ITEM_BAR_CAPACITY; i++)
        {
            Item::Item *bar_item = items_on_item_bar[i];

            if (bar_item != nullptr)
            {
                m_item_slots[i].m_item_id = bar_item->item_id;
                m_item_slots[i].m_item = bar_item;
            }
        }
    }

    void ItemBar::Display(SDL_Renderer *renderer)
    {
        for (ItemSlot &obj : m_item_slots)
        {
            obj.Draw(renderer);
            obj.DisplayIcon(renderer);
        }
    }

    void ItemBar::SelectSlot(int slot_number)
    {
        if (slot_number > 0 && slot_number <= SLOTS_NUMBER)
        {
            if (m_selected_slot != nullptr)
            {
                m_selected_slot->m_id = ITEM_SLOT_ID;

                m_selected_slot = &m_item_slots[slot_number - 1];

                GameContext::selected_item_slot = m_selected_slot;

                m_selected_slot->m_id = SELECTED_ITEM_SLOT_ID;
            }
        }
    }
};
