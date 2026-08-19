#include "../include/gui.h"
#include "../include/textures.h"
#include "../include/id.h"
#include "../include/game_context.h"
#include "../include/inventory.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include <cmath>
#include <string>

constexpr float SLOT_SIZE = 64.0f;

constexpr float ICON_SIZE = 32.0f;
constexpr float ICON_OFFSET = ICON_SIZE * 0.5f;

constexpr float INVETORY_SLOTS_OFFSET = 4.0f;

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
            GUI::ItemSlot({position.x + 0 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 1 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 2 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 3 * SLOT_SIZE, position.y + 16.0f}),
            GUI::ItemSlot({position.x + 4 * SLOT_SIZE, position.y + 16.0f})
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
                // GameContext::selected_item = items_on_item_bar[slot_number - 1];

                m_selected_slot->m_id = SELECTED_ITEM_SLOT_ID;
            }
        }
    }

    Inventory::Inventory(vector2f_t position)
        : GuiObject(position, {0, 0}, 0)
        , m_inventory_slots {}
        , m_hovered_item_slot { nullptr }
        , m_selected_item_slot { nullptr }
    {
        const int SLOTS_IN_ROW = 5;

        UpdateInventory();

        int x;
        int y = 0;

        for (int i=0; i < INVENTORY_CAPACITY; i++)
        {
            x = i % SLOTS_IN_ROW;

            const float X_POSITION = m_position.x + x * SLOT_SIZE;
            const float Y_POSITION = m_position.y + y * SLOT_SIZE;

            m_inventory_slots[i].m_position = {
                X_POSITION,
                Y_POSITION
            };
            m_inventory_slots[i].m_id = ITEM_SLOT_ID;

            m_inventory_slots[i].m_text.m_position = {
                X_POSITION,
                Y_POSITION
            };

            UpdateInventoryView();

            m_inventory_slots[i].m_item_id = items_in_inventory[i].item_id;
            m_inventory_slots[i].m_item = &items_in_inventory[i];

            if (x == SLOTS_IN_ROW - 1) y++;
        }
    }

    void Inventory::UpdateInventoryView()
    {
        for (int i=0; i < INVENTORY_CAPACITY; i++)
        {
            Item::Item *item = &items_in_inventory[i];

                if (item->capacity <= 0)
                {
                    items_in_inventory[i] = Item::Item{0, 0, Item::BLOCK};

                    m_inventory_slots[i].m_item_id = 0;
                }
                else
                {
                    m_inventory_slots[i].m_item_id = items_in_inventory[i].item_id;
                    m_inventory_slots[i].m_item = &items_in_inventory[i];
                }
        }
    }

    void Inventory::Display(SDL_Renderer *renderer)
    {

        // Temporary background
        GuiObject inv_background = GuiObject({m_position.x - 32.0f, m_position.y - 32.0f}, {6 * SLOT_SIZE, 6 * SLOT_SIZE}, ITEM_SLOT_ID);
        inv_background.Draw(renderer);

        for (ItemSlot &slot : m_inventory_slots)
        {
            // If current slot is the hovered one, we store it's original id to change it back to it
            // Terrible GUI draw design... probably will be changed
            if (slot.IsSlotHovered())
            {
                m_hovered_item_slot = &slot;

                int stored_id = slot.m_id;
                slot.m_id = HOVERED_ITEM_SLOT_ID;

                slot.Draw(renderer);
                slot.m_id = stored_id;
            }
            else slot.Draw(renderer);

            slot.DisplayIcon(renderer);
        }
    }

    void Inventory::SelectSlot()
    {
        m_selected_item_slot = m_hovered_item_slot;
        m_selected_item_slot->m_id = SELECTED_ITEM_SLOT_ID;
    }

    void Inventory::MoveItemToHoveredSlot()
    {
        const int selected_slot_index = m_selected_item_slot - &m_inventory_slots[0];
        const int hovered_slot_index = m_hovered_item_slot - &m_inventory_slots[0];

        if (items_in_inventory[selected_slot_index].item_id == 0) return;

        Item::Item tmp_item = items_in_inventory[selected_slot_index];

        items_in_inventory[selected_slot_index] = items_in_inventory[hovered_slot_index];
        items_in_inventory[hovered_slot_index] = tmp_item;

        UpdateInventoryView();
    }

    void Inventory::MapItemToItemBar(int bar_number)
    {
        const int selected_slot_index = m_selected_item_slot - &m_inventory_slots[0];

        if (bar_number < 1 || bar_number > ITEM_BAR_CAPACITY) return;

        items_on_item_bar[bar_number - 1] = &items_in_inventory[selected_slot_index];
        ResetSelectedSlot();
    }

    bool Inventory::IsSlotSelected()
    {
        return (m_selected_item_slot == nullptr) ? false : true;
    }

    void Inventory::ResetSelectedSlot()
    {
        if (m_selected_item_slot != nullptr)
        {
            m_selected_item_slot->m_id = ITEM_SLOT_ID;
            m_selected_item_slot->m_item = nullptr;
            m_selected_item_slot = nullptr;
        }
    }
};
