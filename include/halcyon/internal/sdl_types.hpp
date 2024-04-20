#pragma once

#include <halcyon/utility/concepts.hpp>

#include <SDL3/SDL_rect.h>

// sdl_types.hpp:
// Type aliases for underlying library types.

namespace hal::sdl
{
    constexpr bool integral_coord {
#ifdef HAL_INTEGRAL_COORD
        true
#else
        false
#endif
    };

    using pixel_t = decltype(SDL_Rect::x);
    using coord_t = decltype(SDL_FRect::x);

    template <arithmetic T>
    using point_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;

    template <arithmetic T>
    using rect_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;

    namespace literals
    {
        constexpr sdl::pixel_t operator""_spx(unsigned long long v)
        {
            return static_cast<sdl::pixel_t>(v);
        }

        constexpr sdl::pixel_t operator""_spx(long double v)
        {
            return static_cast<sdl::pixel_t>(v);
        }

        constexpr sdl::coord_t operator""_scrd(unsigned long long v)
        {
            return static_cast<sdl::coord_t>(v);
        }

        constexpr sdl::coord_t operator""_scrd(long double v)
        {
            return static_cast<sdl::coord_t>(v);
        }
    }
}