#include "../include/game_context.h"

namespace GameContext
{
    vector2_t BASE_RESOLUTION = { 1920, 1080 };
    vector2f_t camera = {0.0f, 0.0f};

    float camera_zoom = 2.5f;

    void UpdateCameraPosition(vector2f_t position)
    {
        camera.x = (position.x - BASE_RESOLUTION.x * 0.5f / camera_zoom);
        camera.y = (position.y - BASE_RESOLUTION.y * 0.5f / camera_zoom);
    }
}
