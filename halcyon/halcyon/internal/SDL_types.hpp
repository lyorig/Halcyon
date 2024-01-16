#pragma once

#include <SDL2/SDL_rect.h>
#include <lyo/concepts.hpp>

namespace hal
{
    namespace sdl
    {
        using pixel_t = decltype(SDL_Rect::x);
        using coord_t = decltype(SDL_FRect::x);

        template <lyo::arithmetic T>
        using rect_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;

        template <lyo::arithmetic T>
        using point_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;
    }
}