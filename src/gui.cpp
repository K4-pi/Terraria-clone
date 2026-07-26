#include "../include/gui.h"
#include "../include/textures.h"
#include "../include/id.h"
#include "../include/game_context.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
#include <cmath>

static constexpr float SLOT_SIZE = 64.0f;

static constexpr float ICON_SIZE = 32.0f;
static constexpr float ICON_OFFSET = ICON_SIZE * 0.5f;

namespace GUI
{
    GuiObject::GuiObject(){}

    GuiObject::GuiObject(vector2f_t position, vector2f_t size, int id)
        : m_position { position }
        , m_size { size }
        , m_id { id }
    {}

    void GuiObject::Draw(SDL_Renderer *renderer)
    {
        if (m_id == 0) return;

        texture_coordinates_t texture_source = id_to_gui_texture_dict[m_id];

        SDL_FRect src_rect = {
            .x = texture_source.x,
            .y = texture_source.y,
            .w = texture_source.w,
            .h = texture_source.h,
        };

        SDL_FRect dest_rect = {
            .x = m_position.x,
            .y = m_position.y,
            .w = m_size.x,
            .h = m_size.y,
        };

        if (!SDL_RenderTexture(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect))
        {
            SDL_Log("Failed to draw Entity: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }
    }

    ItemSlot::ItemSlot()
        : GuiObject({0.0f, 0.0f}, {SLOT_SIZE, SLOT_SIZE}, ITEM_SLOT_ID)
        , m_item_id {}
    {}

    ItemSlot::ItemSlot(vector2f_t position)
        : GuiObject(position, {SLOT_SIZE, SLOT_SIZE}, ITEM_SLOT_ID)
        , m_item_id {}
    {}

    bool ItemSlot::IsSlotHovered()
    {
        float x = std::fmin(std::fmax(m_position.x, GameContext::mouse_position.x), m_position.x + m_size.x);
        float y = std::fmin(std::fmax(m_position.y, GameContext::mouse_position.y), m_position.y + m_size.y);

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
    }

    ItemBar::ItemBar(vector2f_t position)
        : GuiObject(position, {0, 0}, ITEM_SLOT_ID)
        , m_item_slots {
            GUI::ItemSlot({position.x + 0 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 1 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 2 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 3 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 4 * SLOT_SIZE, position.y + 16.0f})
        }
        , m_selected_slot { &m_item_slots[0] }
    {

        GameContext::selected_item_slot = m_selected_slot;

        // For testing
        m_item_slots[0].m_item_id = 1;
        m_item_slots[1].m_item_id = 2;
        m_item_slots[2].m_item_id = 3;

        m_selected_slot->m_id = SELECTED_ITEM_SLOT_ID;
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

                GameContext::selected_item_slot = &m_item_slots[slot_number - 1];

                m_selected_slot->m_id = SELECTED_ITEM_SLOT_ID;
            }
        }
    }

    Inventory::Inventory(vector2f_t position)
        : GuiObject(position, {0, 0}, 0)
        , m_inventory_slots{}
    {
        int x;
        int y = 0;

        for (int i=0; i < INVENTORY_CAPACITY; i++)
        {
            x = i % 5;

            m_inventory_slots[i].m_position = {
                m_position.x + x * SLOT_SIZE,
                m_position.y + y * SLOT_SIZE
            };
            m_inventory_slots[i].m_id = ITEM_SLOT_ID;

            if (x == 4) y++;
        }

    }

    void Inventory::Display(SDL_Renderer *renderer)
    {
        for (ItemSlot &slot : m_inventory_slots)
        {
            if (slot.IsSlotHovered()) slot.m_id = HOVERED_ITEM_SLOT_ID;
            else slot.m_id = ITEM_SLOT_ID;

            slot.Draw(renderer);
        }
    }
};
