#pragma once

#include "entity.h"
#include "textures.h"
#include "lmath.h"

#include <vector>

typedef struct
{
    std::vector<texture_coordinates_t> frames;
    float duration;
    int frame_count;
} Animation_t;

class AnimatedSprite : public Entity
{
    private:
        float m_timer;
        int m_current_frame;

    public:
        AnimatedSprite(
            vector2f_t position,
            vector2f_t size,
            int id,
            bool collision=true,
            bool hovered=false
        );
        void Draw(SDL_Renderer *renderer, SDL_FlipMode flip, Animation_t animation, float delta, float angle=0.0f);
};
