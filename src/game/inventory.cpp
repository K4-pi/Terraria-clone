#include "game/inventory.h"
#include "game/id.h"
#include "game/item.h"
#include "engine/gui.h"
#include "game/hud.h"

#include <array>

std::array<Item::Item, INVENTORY_CAPACITY> items_in_inventory = {};
std::array<Item::Item*, ITEM_BAR_CAPACITY> items_on_item_bar = {};

void UpdateInventory()
{
    Item::LoadItemTable();

    items_in_inventory[0] = Item::Item{STONE_BLOCK_ID, 32, Item::BLOCK}; // for testing
    items_in_inventory[1] = Item::Item{GRASS_BLOCK_ID, 32, Item::BLOCK};
    items_in_inventory[2] = Item::Item{DIRT_BLOCK_ID, 32, Item::BLOCK};
    items_in_inventory[3] = Item::Item{FURNACE_ID, 1, Item::BLOCK};

    items_on_item_bar[0] = &items_in_inventory[0]; // for testing
    items_on_item_bar[1] = &items_in_inventory[1];
    items_on_item_bar[2] = &items_in_inventory[2];
    items_on_item_bar[3] = &items_in_inventory[3];
    items_on_item_bar[4] = &items_in_inventory[4];
}

// TODO:
// What if no space in inventory
void AddItemToInvetory(int item_id)
{
    Item::Item *empty_slot = nullptr;

    for (auto &item : items_in_inventory)
    {
        if (item.item_id == item_id)
        {
            item.capacity += 1;
            return;
        }
        else if (item.item_id == 0 && empty_slot == nullptr)
        {
            empty_slot = &item;
        }
    }

    *empty_slot = Item::Item{item_id, 1, Item::BLOCK};
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

        const float X_POSITION = m_position.x + x * GUI::SLOT_SIZE;
        const float Y_POSITION = m_position.y + y * GUI::SLOT_SIZE;

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
    GuiObject inv_background = GuiObject({m_position.x - 32.0f, m_position.y - 32.0f}, {6 * GUI::SLOT_SIZE, 6 * GUI::SLOT_SIZE}, ITEM_SLOT_ID);
    inv_background.Draw(renderer);

    for (GUI::ItemSlot &slot : m_inventory_slots)
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
        m_selected_item_slot = nullptr;
    }
}
