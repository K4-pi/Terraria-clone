#pragma once

#include "block.h"
#include "lmath.h"
#include <SDL3/SDL_render.h>

class Entity 
{
    protected:
        bool CheckCollisionX(Block *b, float delta);
        bool CheckCollisionY(Block *b, float delta);

    public:
        vector2f_t m_position;
        vector2f_t m_size;
        vector2f_t m_velocity;
        RGBA_t m_sprite;
        int m_id;
        bool m_collision;
        bool m_hovered;

        Entity(vector2f_t position, vector2f_t size, RGBA_t sprite, int id, bool collision=true, bool hovered=false);
        void Draw(SDL_Renderer *renderer);
        
};