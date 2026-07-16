#include "../include/animation.h"
#include "../include/lmath.h"
#include "../include/textures.h"
#include "../include/game_context.h"

#include <SDL3/SDL_log.h>

AnimatedSprite::AnimatedSprite(vector2f_t position, vector2f_t size, int id, bool collision)
    : Entity(position, size, id, collision)
    , m_timer { 0.0f }
    , m_current_frame { 0 }
{};

void AnimatedSprite::Draw(SDL_Renderer *renderer, SDL_FlipMode flip, animation_t *animation, float delta, float angle) // Overloads Entity.h
{
    m_timer += delta;

    while (m_timer >= animation->duration)
    {
        m_timer -= animation->duration;
        m_current_frame = (m_current_frame + 1) % animation->frame_count;
    }

    texture_coordinates_t texture_source = animation->frames[m_current_frame];

    SDL_FRect src_rect = {
        .x = texture_source.x,
        .y = texture_source.y,
        .w = texture_source.w,
        .h = texture_source.h,
    };

    vector2f_t local_pos = GetLocalPosition();

    SDL_FRect dest_rect = {
        .x = local_pos.x,
        .y = local_pos.y,
        .w = m_size.x * GameContext::camera_zoom,
        .h = m_size.y * GameContext::camera_zoom,
    };

    if (!SDL_RenderTextureRotated(renderer, TEXTURES_TILEMAP, &src_rect, &dest_rect, angle, NULL, flip))
    {
        SDL_Log("Failed to draw AnimatedSprite: %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
