#pragma once

#include <string_view>

#include <SDL_blendmode.h>

#include <halcyon/types/rectangle.hpp>

// types/render.hpp:
// Rendering-related types used throughout Halcyon.
// Defining HAL_FAST_TYPES maps Halcyon's types directly to SDL's where applicable.

namespace hal
{
    enum class blend_mode : u8
    {
        none  = SDL_BLENDMODE_NONE,
        add   = SDL_BLENDMODE_ADD,
        mod   = SDL_BLENDMODE_MOD,
        mul   = SDL_BLENDMODE_MUL,
        blend = SDL_BLENDMODE_BLEND
    };

    constexpr std::string_view to_string(blend_mode bm)
    {
        switch (bm)
        {
            using enum blend_mode;

        case none:
            return "None";

        case blend:
            return "Blend";

        case add:
            return "Add";

        case mod:
            return "Mod";

        case mul:
            return "Mul";
        }
    }

    // Defining HAL_FAST_TYPES transforms these into native SDL types.

    // A measurement unit for surfaces, textures, displays etc.
    using pixel_t = detail::cft<i32, sdl::pixel_t>;

    // A measurement unit for sub-pixel rendering of textures.
    using coord_t = detail::cft<f32, sdl::coord_t>;

    using pixel_point = point<pixel_t>;
    using pixel_rect  = rectangle<pixel_t>;

    using coord_point = point<coord_t>;
    using coord_rect  = rectangle<coord_t>;

    namespace literals
    {
        constexpr pixel_t operator""_px(unsigned long long v)
        {
            return static_cast<pixel_t>(v);
        }

        constexpr pixel_t operator""_px(long double v)
        {
            return static_cast<pixel_t>(v);
        }

        constexpr coord_t operator""_crd(unsigned long long v)
        {
            return static_cast<coord_t>(v);
        }

        constexpr coord_t operator""_crd(long double v)
        {
            return static_cast<coord_t>(v);
        }
    }

    namespace sdl
    {
        template <meta::arithmetic T>
        using point_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;

        template <meta::arithmetic T>
        using rect_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;
    }

    // Paranoia.
    static_assert(std::is_signed_v<pixel_t>);
    static_assert(sizeof(sdl::pixel_point) == sizeof(SDL_Point) && sizeof(sdl::coord_point) == sizeof(SDL_FPoint) && sizeof(sdl::pixel_rect) == sizeof(SDL_Rect) && sizeof(sdl::coord_rect) == sizeof(SDL_FRect));
}