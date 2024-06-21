#pragma once

#include <SDL_rect.h>

#include <halcyon/internal/literals.hpp>
#include <halcyon/utility/concepts.hpp>

// internal/sdl_types.hpp:
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

    namespace literals
    {
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(pixel_t, px);
        HAL_DETAIL_DEFINE_LITERAL_OPERATOR(coord_t, crd);
    }
}