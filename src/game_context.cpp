#include "../include/game_context.h"

namespace GameContext
{
    vector2_t BASE_RESOLUTION = { 1920, 1080 };
    vector2f_t camera = { 0.0f, 0.0f };
    vector2_t world_size = { 316, 316 };

    float camera_zoom = 2.0f;

    void UpdateCameraPosition(vector2f_t position)
    {
        float pos_x = (position.x - BASE_RESOLUTION.x * 0.5f / camera_zoom);
        float pos_y = (position.y - BASE_RESOLUTION.y * 0.5f / camera_zoom);

        float map_edge = world_size.x * 16.0f;
        float map_bottom = world_size.y * 16.0f;

        vector2f_t world_viewport = {
            BASE_RESOLUTION.x / camera_zoom,
            BASE_RESOLUTION.y / camera_zoom,
        };

        if (pos_x > 0 && pos_x + world_viewport.x < map_edge) camera.x = pos_x;
        else camera.x = (pos_x < 16.0f) ? 0.0f : map_edge - world_viewport.x;

        if (pos_y > 0 && pos_y + world_viewport.y < map_bottom) camera.y = pos_y;
        else camera.y = (pos_y < 16.0f) ? 0.0f : map_bottom - world_viewport.y;
    }
}
