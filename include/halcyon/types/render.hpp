#pragma once

#include <SDL_blendmode.h>
#include <halcyon/types/rectangle.hpp>

// types/render.hpp:
// Rendering-related types used throughout Halcyon.
// Defining LYO_FAST_TYPES maps Halcyon's types directly to SDL's where applicable.

namespace hal
{
    enum class blend_mode : lyo::u8
    {
        none  = SDL_BLENDMODE_NONE,
        add   = SDL_BLENDMODE_ADD,
        mod   = SDL_BLENDMODE_MOD,
        mul   = SDL_BLENDMODE_MUL,
        blend = SDL_BLENDMODE_BLEND
    };

    // A measurement type for surface/texture pixels.
    using pixel_t = lyo::CFT<lyo::i16, sdl::pixel_t>;

    // A drawing coordinate. Floating point by default, but you can define HAL_INTEGRAL_COORD to
    // make it be integral and potentially improve performance.
    using coord_t = lyo::CFT<std::conditional_t<sdl::integral_coord, pixel_t, lyo::f32>, sdl::coord_t>;

    using pixel_point = point<pixel_t>;
    using pixel_rect  = rectangle<pixel_t>;

    using coord_point = point<coord_t>;
    using coord_rect  = rectangle<coord_t>;

    namespace literals
    {
        constexpr pixel_t operator""_px(unsigned long long v)
        {
            return lyo::cast<pixel_t>(v);
        }

        constexpr pixel_t operator""_px(long double v)
        {
            return lyo::cast<pixel_t>(v);
        }

        constexpr coord_t operator""_crd(unsigned long long v)
        {
            return lyo::cast<coord_t>(v);
        }

        constexpr coord_t operator""_crd(long double v)
        {
            return lyo::cast<coord_t>(v);
        }
    }

    // Paranoia.
    static_assert(std::is_signed_v<pixel_t>);
    static_assert(sizeof(sdl::pixel_point) == sizeof(SDL_Point) && sizeof(sdl::coord_point) == sizeof(SDL_FPoint) && sizeof(sdl::pixel_rect) == sizeof(SDL_Rect) && sizeof(sdl::coord_rect) == sizeof(SDL_FRect));
}