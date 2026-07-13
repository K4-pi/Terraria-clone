#pragma once

#include "lmath.h"

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>

class Entity
{

    public:
        vector2f_t m_position;
        vector2f_t m_velocity;
        vector2f_t m_size;
        int m_id;
        bool m_collision;
        bool m_hovered;

        Entity(vector2f_t position, vector2f_t size, int id, bool collision=true, bool hovered=false);
        void Draw(SDL_Renderer *renderer, SDL_FlipMode flip=SDL_FLIP_NONE, float angle=0.0);
        vector2f_t GetLocalPosition();
};
