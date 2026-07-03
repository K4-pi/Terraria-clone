#include <algorithm>
#include <memory>
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

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
#include <iostream>
#include <vector>
#include <cmath>

#include "include/id.h"
#include "include/lmath.h"
#include "include/player.h"
#include "include/block.h"

#define GREEN  {0, 255, 0, 0}
#define RED    {255, 0, 0, 0}
#define HOVER  {128, 128, 128, 0}
#define PURPLE {128, 0, 128, 0}

static constexpr vector2_t BASE_RESOLUTION = { 1920, 1080 };
// static constexpr vector2_t BASE_RESOLUTION = { 1280, 720 };

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_MouseButtonFlags mouse_buttons;
static vector2f_t mouse_position;

std::vector<Block> blocks(128 * 128);

SDL_Surface *dirt_surface;
SDL_Texture *dirt_tex;

SDL_Surface *hover_surface;
SDL_Texture *hover_tex;

Block* hovered_block = nullptr;

static Uint64 last_tick;

static Player player = Player({100.0f, 100.0f}, {32.0f, 32.0f}, RED, PLAYER, 200.0f, true);

vector2f_t Camera = {0, 0};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", BASE_RESOLUTION.x, BASE_RESOLUTION.y, SDL_WINDOW_RESIZABLE, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, BASE_RESOLUTION.x, BASE_RESOLUTION.y, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    // Blocks initialization
    int y = 0;
    int x = 0;
    for (int i=0; i < blocks.size(); i++)
    {
        if ((x = i % 128) == 0) y++;

        blocks[i] = Block
        {
            {50.0f + x * 32.0f, 100.0f + y * 32.0f},
            {32.0f, 32.0f},
            GREEN,
            GRASS_BLOCK_ID
        };
    }

    last_tick = SDL_GetTicks();

    dirt_surface = SDL_LoadPNG("Sprites/dirt.png");
    dirt_tex     = SDL_CreateTextureFromSurface(renderer, dirt_surface);

    hover_surface = SDL_LoadPNG("Sprites/hover.png");
    hover_tex     = SDL_CreateTextureFromSurface(renderer, hover_surface);

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
    }

    mouse_buttons = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_RenderCoordinatesFromWindow(renderer, mouse_position.x, mouse_position.y, &mouse_position.x, &mouse_position.y);

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (mouse_buttons & SDL_BUTTON_LMASK)
        {
            std::cout << "left mouse pressed" << std::endl;

            if (hovered_block != nullptr)
            {
                std::cout << "Hovered block = " << hovered_block << std::endl;

                hovered_block->m_id = SKY_BLOCK_ID;
                hovered_block->m_collision = false;
            }

        }
        if (mouse_buttons & SDL_BUTTON_RMASK)
        {
            std::cout << "right mouse pressed" << std::endl;

            if (hovered_block != nullptr)
            {
                std::cout << "Hovered block = " << hovered_block << std::endl;

                hovered_block->m_id = GRASS_BLOCK_ID;
                hovered_block->m_collision = true;
            }
        }
    }

    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        switch (event->key.key)
        {
            case SDLK_D:
                player.keys_pressed.D = true;
                std::cout << "D pressed" << std::endl;
                break;

            case SDLK_A:
                player.keys_pressed.A = true;
                std::cout << "A pressed" << std::endl;
                break;

            case SDLK_SPACE:
                player.keys_pressed.SPACE = true;
                std::cout << "Space pressed" << std::endl;
                break;

            default:
                std::cout << "Not supported key" << std::endl;
                break;
        }
    }

    else if (event->type == SDL_EVENT_KEY_UP)
    {
        switch (event->key.key)
        {
            case SDLK_D:
                player.keys_pressed.D = false;
                std::cout << "D unpressed" << std::endl;
                break;

            case SDLK_A:
                player.keys_pressed.A = false;
                std::cout << "A unpressed" << std::endl;
                break;

            case SDLK_SPACE:
                player.keys_pressed.SPACE = false;
                std::cout << "Space unpressed" << std::endl;
                break;

            default:
                std::cout << "Not supported key" << std::endl;
                break;
        }
    }

    player.HandleInput();

    return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    Uint64 current_tick = SDL_GetTicks();

    float delta_time = (current_tick - last_tick) / 1000.0f;
    last_tick = current_tick;

    player.MovePlayer(delta_time, blocks);

    Camera.x = player.m_position.x - BASE_RESOLUTION.x * 0.5f;
    Camera.y = player.m_position.y - BASE_RESOLUTION.y * 0.5f;

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 128, 196, 0);
    SDL_RenderClear(renderer);

    // Draw here

    for (size_t i=0; i < blocks.size(); i++)
    {
        Block *current_block = &blocks[i];

        bool hover_x = std::fmax(std::fmin(mouse_position.x, current_block->m_position.x + current_block->m_size.x - Camera.x), current_block->m_position.x - Camera.x) == mouse_position.x;
        bool hover_y = std::fmax(std::fmin(mouse_position.y, current_block->m_position.y + current_block->m_size.y - Camera.y), current_block->m_position.y - Camera.y) == mouse_position.y;

        if (hover_x && hover_y)
        {
            current_block->m_hovered = true;

            hovered_block = current_block;
        }
        else
        {
            current_block->m_hovered = false;
        }

        SDL_FRect rect = {
            .x = current_block->m_position.x - Camera.x,
            .y = current_block->m_position.y - Camera.y,
            .w = current_block->m_size.x,
            .h = current_block->m_size.y,
        };

        if (current_block->m_position.x + current_block->m_size.x > Camera.x &&
            current_block->m_position.x < Camera.x + BASE_RESOLUTION.x &&
            current_block->m_position.y > Camera.y &&
            current_block->m_position.y < Camera.y + BASE_RESOLUTION.y && current_block->m_id == GRASS_BLOCK_ID)
        {
            // SDL_RenderFillRect(renderer, &rect);

            SDL_RenderTexture(renderer, dirt_tex, nullptr, &rect);
        }

        if (current_block->m_hovered)
        {
            SDL_RenderTexture(renderer, hover_tex, nullptr, &rect);
        }
    }

    player.Draw(renderer, {player.m_position.x - Camera.x, player.m_position.y - Camera.y});

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_DestroySurface(dirt_surface);
    SDL_DestroyTexture(dirt_tex);
    /* SDL will clean up the window/renderer for us. */
}
