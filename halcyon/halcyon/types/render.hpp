#pragma once

#include <halcyon/types/point.hpp>
#include <halcyon/types/rectangle.hpp>

// render.hpp:
// Rendering-related types used throughout Halcyon.

namespace hal
{
    // Native SDL interop ought to be the fastest.
    // As such, using fast types will map Halcyon's types to SDL's.
    using pixel_t = lyo::CFT<lyo::i16, sdl::pixel_t>;
    using coord_t = lyo::CFT<lyo::f64, sdl::coord_t>;

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
}

// Paranoia.
static_assert(std::is_signed_v<hal::pixel_t>, "Pixel type must be signed");
static_assert(std::is_floating_point_v<hal::coord_t>, "Position type must be floating point");

static_assert(sizeof(hal::sdl::pixel_point) == sizeof(SDL_Point) && sizeof(hal::sdl::coord_point) == sizeof(SDL_FPoint) && sizeof(hal::sdl::pixel_rect) == sizeof(SDL_Rect) && sizeof(hal::sdl::coord_rect) == sizeof(SDL_FRect), "Incompatible memory layout between SDL structs and their wrappers");