#pragma once

#include "gui.h"
#include "item.h"
#include "lmath.h"

#include <SDL3_ttf/SDL_ttf.h>

namespace GameContext
{
    constexpr float STANDARD_BLOCK_SIZE = 16.0f;
    constexpr float FONT_SIZE = 24.0f;

    extern vector2_t BASE_RESOLUTION;
    extern vector2_t world_size;
    extern vector2f_t camera;
    extern vector2f_t mouse_position;
    extern float camera_zoom;

    extern TTF_Font *text_font;

    extern GUI::ItemSlot *selected_item_slot;
    // extern Item::Item *selected_item;

    void UpdateCameraPosition(vector2f_t position);
}
