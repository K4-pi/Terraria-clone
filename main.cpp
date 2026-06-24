#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */

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

static constexpr vector2_t BASE_RESOLUTION = {1920, 1080};

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_MouseButtonFlags mouse_buttons;
static vector2f_t mouse_position;

std::vector<Block> blocks(128 * 128);

SDL_Texture *grass_tex;

Block *hovered_block;

static Uint64 last_tick;

static Player player = Player({100.0f, 100.0f}, {32.0f, 32.0f}, RED, PLAYER, 200.0f, true);

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

    int y = 0;
    int x = 0;
    for (int i=0; i < blocks.size(); i++)
    {
        if ((x = i % 128) == 0) y++;

        if (y < 8)
        {
            blocks[i] = Block
            {
                {50.0f + x * 32.0f, 100.0f + y * 32.0f},
                {32.0f, 32.0f}, {0, 128, 196, 0},
                SKY_BLOCK_ID,
                false,
                false
            };
        }
        else
        {
            blocks[i] = Block
            {
                {50.0f + x * 32.0f, 100.0f + y * 32.0f},
                {32.0f, 32.0f},
                GREEN,
                GRASS_BLOCK_ID
            };
        }
    }

    hovered_block = &blocks[16 * 16];

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

    mouse_buttons = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
    SDL_RenderCoordinatesFromWindow(renderer, mouse_position.x, mouse_position.y, &mouse_position.x, &mouse_position.y);

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        if (mouse_buttons & SDL_BUTTON_LMASK)
        {
            std::cout << "left mouse pressed" << std::endl;

            if (hovered_block != nullptr)
                *hovered_block = Block
                {
                    {hovered_block->m_position.x, hovered_block->m_position.y},
                    {32.0f, 32.0f}, {0, 128, 196, 0},
                    SKY_BLOCK_ID,
                    false,
                    false
                };
        }
        if (mouse_buttons & SDL_BUTTON_RMASK)
        {
            std::cout << "right mouse pressed" << std::endl;

            if (hovered_block->m_id == SKY_BLOCK_ID)
                *hovered_block = Block
                {
                    {hovered_block->m_position.x, hovered_block->m_position.y},
                    {32.0f, 32.0f},
                    PURPLE,
                    GRASS_BLOCK_ID
                };
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

    // std::cout << "Mouse postion = " << mouse_position.x << ", " << mouse_position.y << std::endl;

    player.MovePlayer(delta_time, blocks);

    // if (player.is_grounded) std::cout << "Player is grounded" << std::endl;
    // else std::cout << "Player is not grounded" << std::endl;

    // printf("Velocity = %f, %f\n", player.velocity.x, player.velocity.y);

    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 128, 196, 0);
    SDL_RenderClear(renderer);

    // Draw here

    // dont redraw everytime, only when player changes blocks

    for (size_t i=0; i < blocks.size(); i++)
    {
        bool hover_x = std::fmax(std::fmin(mouse_position.x, blocks[i].m_position.x + blocks[i].m_size.x), blocks[i].m_position.x) == mouse_position.x;
        bool hover_y = std::fmax(std::fmin(mouse_position.y, blocks[i].m_position.y + blocks[i].m_size.y), blocks[i].m_position.y) == mouse_position.y;

        if (hover_x && hover_y)
        {
            blocks[i].m_hovered = true;

            hovered_block = &blocks[i];
            SDL_SetRenderDrawColor(renderer, 128, 128, 128, 100);
        }
        else
        {
            blocks[i].m_hovered = false;
            SDL_SetRenderDrawColor(renderer, blocks[i].m_sprite.r, blocks[i].m_sprite.g, blocks[i].m_sprite.b, 0);
        }

        SDL_FRect rect = {
            .x = blocks[i].m_position.x,
            .y = blocks[i].m_position.y,
            .w = blocks[i].m_size.x,
            .h = blocks[i].m_size.y,
        };

        SDL_RenderFillRect(renderer, &rect);
    }

    player.Draw(renderer);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);

    return SDL_APP_CONTINUE;  // Continue
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
