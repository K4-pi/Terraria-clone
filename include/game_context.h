#pragma once

#include "lmath.h"

namespace GameContext
{
    constexpr float STANDARD_BLOCK_SIZE = 16.0f;

    extern vector2_t BASE_RESOLUTION;
    extern vector2f_t camera;
    extern float camera_zoom;
    extern vector2_t world_size;

    void UpdateCameraPosition(vector2f_t position);
}
