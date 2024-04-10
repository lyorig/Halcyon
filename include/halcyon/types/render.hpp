#pragma once

#include <halcyon/types/rectangle.hpp>

// render.hpp:
// Rendering-related types used throughout Halcyon.

namespace hal
{
    // Defining LYO_FAST_TYPES maps Halcyon's types directly to SDL's where applicable.

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
    static_assert(std::is_signed_v<pixel_t>, "Pixel type must be signed");
    static_assert(sizeof(sdl::pixel_point) == sizeof(SDL_Point) && sizeof(sdl::coord_point) == sizeof(SDL_FPoint) && sizeof(sdl::pixel_rect) == sizeof(SDL_Rect) && sizeof(sdl::coord_rect) == sizeof(SDL_FRect), "Incompatible memory layout between SDL structs and their wrappers");
}