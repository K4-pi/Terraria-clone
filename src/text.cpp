#include "../include/text.h"
#include "../include/game_context.h"

#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_surface.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <cstddef>
#include <cstdlib>

static constexpr SDL_Color TEXT_COLOR = {255, 255, 255, 0};

namespace Text
{
    Text::Text(vector2f_t position)
        : Entity(position, {0.0f, 0.0f}, 0)
    {}

    void Text::Draw(SDL_Renderer *renderer, std::string text)
    {
        int text_len = text.size();

        SDL_Surface *text_surface = TTF_RenderText_Solid(GameContext::text_font, text.c_str(), text_len, TEXT_COLOR);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        SDL_DestroySurface(text_surface);

        SDL_FRect rect = {
            m_position.x,
            m_position.y,
            GameContext::FONT_SIZE * text_len,
            GameContext::FONT_SIZE
        };

        if (!SDL_RenderTexture(renderer, texture, NULL, &rect))
        {
            SDL_Log("Failed to draw text: %s", SDL_GetError());
            exit(EXIT_FAILURE);
        }

        SDL_DestroyTexture(texture);
    }

}
