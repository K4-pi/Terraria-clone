#pragma once

#include "engine/lmath.h"

#include <SDL3/SDL_render.h>
#include <functional>

namespace GUI
{
    class GuiObject
    {
        public:
            vector2f_t m_position;
            vector2f_t m_size;
            int m_id;

            GuiObject();
            GuiObject(vector2f_t position, vector2f_t size, int id);
            void Draw(SDL_Renderer *renderer);
    };

    class Button : public GuiObject
    {
        private:
            std::function<void()> click_function;

        public:
            Button(vector2f_t position, vector2f_t size, int id);
            void OnClick(std::function<void()> fn);
            void ButtonListener();
    };
};
