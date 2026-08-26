#include "engine/gui.h"
#include "engine/textures.h"
#include "game/game_context.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_log.h>

#include <algorithm>
#include <cstdlib>
#include <functional>

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

    Button::Button(vector2f_t position, vector2f_t size, int id)
        : GuiObject(position, size, id)
        , click_function {}
    {}

    void Button::OnClick(std::function<void()> fn)
    {
       click_function = std::move(fn);
    }

    void Button::ButtonListener()
    {
        bool hover_x = std::min(std::max(GameContext::mouse_position.x, m_position.x), m_position.x + m_size.x) == GameContext::mouse_position.x;
        bool hover_y = std::min(std::max(GameContext::mouse_position.y, m_position.y), m_position.y + m_size.y) == GameContext::mouse_position.y;

        if ((hover_x && hover_y) && click_function) click_function();
    }
};
