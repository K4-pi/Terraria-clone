#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_surface.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_mouse.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>

#include <cstdio>
#include <cstddef>
#include <cstdlib>
#include <vector>

#include "include/textures.h"
#include "include/id.h"
#include "include/lmath.h"
#include "include/player.h"
#include "include/world.h"
#include "include/game_context.h"
#include "include/input.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static Uint64 last_tick;

static vector2f_t mouse_position;

static Player player = Player({120.0f * 16.0f, 157.0f * 16.0f}, {16.0f, 16.0f}, PLAYER, 200.0f, true);
static World world = World();

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", GameContext::BASE_RESOLUTION.x, GameContext::BASE_RESOLUTION.y, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, GameContext::BASE_RESOLUTION.x, GameContext::BASE_RESOLUTION.y, SDL_LOGICAL_PRESENTATION_OVERSCAN);

    if (LoadTexturesFromFile(renderer, "tilemap.png") == -1)
    {
        SDL_Log("Failed to load textures from a file: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    last_tick = SDL_GetTicks();

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_RenderCoordinatesFromWindow(renderer, mouse_position.x, mouse_position.y, &mouse_position.x, &mouse_position.y);

    Input::HandleEvent(*event);

    player.HandleInput(Input::state);

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 current_tick = SDL_GetTicks();

    float delta_time = (current_tick - last_tick) / 1000.0f;
    last_tick = current_tick;

    player.MovePlayer(delta_time, world.GetBlocks());

    GameContext::UpdateCameraPosition(player.m_position);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 128, 196, 0);
    SDL_RenderClear(renderer);

    // Draw here
    world.UpdateHoveredBlock(mouse_position);

    if (Input::state.mouse_left_clicked)
    {
        world.SetHoveredBlock(SKY_BLOCK_ID, false);  // Destroy block
    }

    if (Input::state.mouse_right_clicked)
    {
        world.SetHoveredBlock(GRASS_BLOCK_ID, true);  // Place block
    }

    world.DrawWorld(renderer);

    player.Draw(renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);
    Input::EndFrame();

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    DeleteTextures();
    /* SDL will clean up the window/renderer for us. */
}
