#pragma once

#include <SDL3/SDL_events.h>

#include "engine/input_state.h"

namespace Input
{
    extern InputState state;

    void HandleEvent(const SDL_Event& event);
    void EndFrame();
}
