#pragma once

#include <halcyon/types/point.hpp>
#include <halcyon/types/rectangle.hpp>

// render.hpp:
// Rendering-related types used throughout Halcyon.

namespace hal
{
    // Native SDL interop ought to be the fastest.
    // As such, using fast types will map Halcyon's types to SDL's.
    using pixel_t = lyo::CFT<lyo::i16, SDL::pixel_t>;
    using coord_t = lyo::CFT<lyo::f64, SDL::coord_t>;

    using pixel_size = point<pixel_t>;
    using pixel_pos  = point<pixel_t>;
    using pixel_area = rectangle<pixel_t>;

    using coord      = point<coord_t>;
    using coord_area = rectangle<coord_t>;
}

// Paranoia.
static_assert(std::is_signed_v<hal::pixel_t>, "Pixel type must be signed");
static_assert(std::is_floating_point_v<hal::coord_t>, "Position type must be floating point");

static_assert(sizeof(hal::SDL::Point) == sizeof(SDL_Point) && sizeof(hal::SDL::FPoint) == sizeof(SDL_FPoint) && sizeof(hal::SDL::Rect) == sizeof(SDL_Rect) && sizeof(hal::SDL::FRect) == sizeof(SDL_FRect), "Incompatible memory layout between SDL structs and their wrappers");