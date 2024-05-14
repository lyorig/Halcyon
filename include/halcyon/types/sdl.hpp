#pragma once

#include <halcyon/utility/concepts.hpp>

#include <SDL_rect.h>

// types/sdl.hpp:
// Type aliases for underlying library types.

namespace hal
{
    namespace sdl
    {
        using pixel_t = decltype(SDL_Rect::x);
        using coord_t = decltype(SDL_FRect::x);

        template <meta::arithmetic T>
        using point_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;

        template <meta::arithmetic T>
        using rect_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;
    }
}