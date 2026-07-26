#pragma once

#include "gui.h"
#include "lmath.h"

namespace GameContext
{
    constexpr float STANDARD_BLOCK_SIZE = 16.0f;

    extern vector2_t BASE_RESOLUTION;
    extern vector2f_t camera;
    extern float camera_zoom;
    extern vector2_t world_size;

    extern GUI::ItemSlot *selected_item_slot;

    void UpdateCameraPosition(vector2f_t position);
}
