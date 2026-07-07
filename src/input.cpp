#include "../include/input.h"

#include <SDL3/SDL_keycode.h>

namespace Input
{
    InputState state = {
        false, false, false,
        false, false,
        false, false
    };

    void HandleEvent(const SDL_Event& event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            switch (event.key.key)
            {
                case SDLK_A: state.move_left = true; break;
                case SDLK_D: state.move_right = true; break;
                case SDLK_SPACE: state.jump = true; break;
                default: break;
            }
        }
        else if (event.type == SDL_EVENT_KEY_UP)
        {
            switch (event.key.key)
            {
                case SDLK_A: state.move_left = false; break;
                case SDLK_D: state.move_right = false; break;
                case SDLK_SPACE: state.jump = false; break;
                default: break;
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                state.mouse_left_down = true;
                state.mouse_left_clicked = true;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                state.mouse_right_down = true;
                state.mouse_right_clicked = true;
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
        {
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                state.mouse_left_down = false;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                state.mouse_right_down = false;
            }
        }
    }

    void EndFrame()
    {
        state.mouse_left_clicked = false;
        state.mouse_right_clicked = false;
    }
}
