#include "../include/gui.h"
#include "../include/textures.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>
#include <cstddef>

static constexpr float SLOT_SIZE = 64.0f;

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

    ItemBar::ItemBar(vector2f_t position, int id)
        : GuiObject(position, {SLOT_SIZE * SLOTS_NUMBER, SLOT_SIZE}, id)
        , m_item_slots {}
    {
        std::size_t i = 0;

        for (GuiObject &obj : m_item_slots)
        {
            obj = GuiObject({position.x + i * SLOT_SIZE, position.y + 16.0f},  {SLOT_SIZE, SLOT_SIZE}, id);
            i++;
        }
    }

    void ItemBar::Display(SDL_Renderer *renderer)
    {
        for (GuiObject &obj : m_item_slots)
        {
            obj.Draw(renderer);
        }
    }
};
