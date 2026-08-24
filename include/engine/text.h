#pragma once

#include "engine/entity.h"

#include <SDL3_ttf/SDL_ttf.h>

#include <string>

namespace Text
{
    class Text : public Entity
    {
        public:
            Text(vector2f_t position);
            void Draw(SDL_Renderer *renderer, std::string text);
    };
}
