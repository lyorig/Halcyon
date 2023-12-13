#pragma once

#include <SDL2/SDL_rect.h>
#include <lyo/concepts.hpp>

namespace hal
{
    namespace SDL
    {
        using pixel_type = decltype(SDL_Rect::x);
        using position_type = decltype(SDL_FRect::x);

        template <lyo::arithmetic T>
        using rect_type = std::conditional_t<std::is_same_v<T, pixel_type>, SDL_Rect, SDL_FRect>;

        template <lyo::arithmetic T>
        using point_type = std::conditional_t<std::is_same_v<T, pixel_type>, SDL_Point, SDL_FPoint>;
    }
}