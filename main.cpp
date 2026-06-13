#include "include/lmath.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_oldnames.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "include/player.h"
#include "include/block.h"

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

std::vector<Block> blocks;

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
        .is_grounded = false,
        .max_speed = 100.0,
        .body = {32.0, 32.0},
        .position = {100.0f, 100.0f},
        .velocity = {0.0, 0.0},
        .direction = {0, 0},
    };

    blocks.push_back(Block ({275.0f, 100.0f}, {32.0f, 32.0f}, "grass", true));
    blocks.push_back(Block ({50.0f, 100.0f}, {32.0f, 32.0f}, "grass", true));

    for (int i = 0; i < 9; i++)
    {
        blocks.push_back(Block ({50.0f + 32.0f * i, 164.0f}, {32.0f, 32.0f}, "grass", true));
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

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.key) {
        
            case SDLK_D:
                player.direction.x = 1;
                std::cout << "D pressed" << std::endl;
                break;

            case SDLK_A:
                player.direction.x = -1;
                std::cout << "A pressed" << std::endl;
                break;

            default:
                std::cout << "Not supported key" << std::endl;
                break;
        }
        
        if (event->key.key == SDLK_SPACE)
        {
            // if (player.is_grounded)
            // {
            //     player.direction.y = -1;
            // }

            std::cout << "Space pressed" << std::endl;
        }
    }

    if (event->type == SDL_EVENT_KEY_UP)
    {
        player.direction.x = 0;
    }

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 current_tick = SDL_GetTicks();

    float delta_time = (current_tick - last_tick) / 1000.0f;
    last_tick = current_tick;

    MovePlayer(&player, delta_time, blocks);

    // if (player.is_grounded) std::cout << "Player is grounded" << std::endl;
    // else std::cout << "Player is not grounded" << std::endl;

    printf("Velocity = %f, %f\n", player.velocity.x, player.velocity.y);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    // Draw here

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    
    for (Block b : blocks)
    {
        SDL_FRect rect = {
            .x = b.m_position.x,
            .y = b.m_position.y,
            .w = b.m_size.x,
            .h = b.m_size.y,
        };

        SDL_RenderFillRect(renderer, &rect);
    }

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
