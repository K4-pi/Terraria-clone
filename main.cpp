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
#include "include/lmath.h"
#include "include/player.h"
#include "include/world.h"
#include "include/game_context.h"
#include "include/input.h"
#include "include/animation.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static bool is_window_fullscreen = false;

static Uint64 last_tick;

static vector2f_t mouse_position;

static Player player = Player({120.0f * 16.0f, 157.0f * 16.0f}, {30.0f, 45.0f}, 112, 200.0f, true);
static World world = World();

static animation_t anim_player_idle;
static animation_t anim_player_walk;
static animation_t anim_player_jump;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Game", "1.0", "com.example.game");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("survival game", GameContext::BASE_RESOLUTION.x, GameContext::BASE_RESOLUTION.y, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, GameContext::BASE_RESOLUTION.x, GameContext::BASE_RESOLUTION.y, SDL_LOGICAL_PRESENTATION_OVERSCAN);

    if (LoadTexturesFromFile(renderer, "resources/tilemap.png") == -1)
    {
        SDL_Log("Failed to load textures from a file: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    last_tick = SDL_GetTicks();

    anim_player_idle = {
        .frames = animation_dict["player_idle"],
        .duration = 0.5f,
        .frame_count = 2,
    };

    anim_player_walk = {
        .frames = animation_dict["player_walk"],
        .duration = 0.25f,
        .frame_count = 2,
    };

    anim_player_jump = {
        .frames = animation_dict["player_jump"],
        .duration = 1.0f,
        .frame_count = 2,
    };

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

    if (Input::state.f11)
    {
        is_window_fullscreen = is_window_fullscreen ? false : true;

        SDL_SetWindowFullscreen(window, is_window_fullscreen);
        Input::state.f11 = false;
    }
    if (Input::state.number_1 && GameContext::camera_zoom < 5.0f)
    {
        GameContext::camera_zoom += 1.0f;
        Input::state.number_1 = false;
    }
    if (Input::state.number_2 && GameContext::camera_zoom > 1.0f)
    {
        GameContext::camera_zoom -= 1.0f;
        Input::state.number_2 = false;
    }

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
    SDL_SetRenderDrawColor(renderer, 36, 140, 190, 0);
    SDL_RenderClear(renderer);

    // Draw here
    world.UpdateHoveredBlock(mouse_position);

    player.ModifyHoverBlock(&world, delta_time);

    world.DrawWorld(renderer);

    // Determine animation
    animation_t *current_animation;

    if (player.m_velocity.y != 0.0f)
    {
        current_animation = &anim_player_jump;
    }
    else if (player.m_velocity.x != 0.0f)
    {
        current_animation = &anim_player_walk;
    }
    else
    {
        current_animation = &anim_player_idle;
    }

    // Draw player in looking direction
    if (player.GetLocalPosition().x > mouse_position.x) player.Draw(renderer, SDL_FLIP_NONE, current_animation, delta_time);
    else player.Draw(renderer, SDL_FLIP_HORIZONTAL, current_animation, delta_time);

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
