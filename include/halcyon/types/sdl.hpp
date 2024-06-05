#pragma once

#include <halcyon/utility/concepts.hpp>

#include <SDL_rect.h>

// types/sdl.hpp:
// Type aliases for base library types.
// Also prevents circular includes in other files.

namespace hal
{
    using pixel_t = decltype(SDL_Rect::x);
    using coord_t = decltype(SDL_FRect::x);

    // Paranoia.
    static_assert(std::is_signed_v<pixel_t>);

    namespace detail
    {
        template <meta::arithmetic T>
        using sdl_point = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;

        template <meta::arithmetic T>
        using sdl_rect = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;
    }
}