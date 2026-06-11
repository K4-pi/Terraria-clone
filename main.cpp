#include <SDL3/SDL_stdinc.h>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL_timer.h>
#include <cmath>
#include <cstdlib>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <cstdio>

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "include/player.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static player_t player;

static Uint64 last_tick;

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO)) 
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer)) 
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    player = {
        .max_speed = 100.0,
        .body = {32.0, 32.0},
        .position = {100.0, 100.0},
        .velocity = {0.0, 0.0},
        .direction = {0, 0},
    };

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

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.key == SDLK_D) 
        {
            player.direction.x = 1;
            printf("D pressed\n");
        }
        else if (event->key.key == SDLK_A) 
        {
            player.direction.x = -1;
            printf("A pressed\n");
        }
    }

    if (event->type == SDL_EVENT_KEY_UP)
    {
        player.direction = {0, 0};
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 current_tick = SDL_GetTicks();

    float delta_time = (current_tick - last_tick) / 1000.0f;
    last_tick = current_tick;

    MovePlayer(&player, delta_time);
    
    printf("Velocity = %fx\n", player.velocity.x);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Draw here
    DrawPlayer(renderer, &player);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
