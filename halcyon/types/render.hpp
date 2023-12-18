#pragma once

#include <lyo/slider.hpp>
#include <lyo/types.hpp>

#include "point.hpp"
#include "rectangle.hpp"
#include <halcyon/internal/SDL_types.hpp>

/* render.hpp:
   Rendering-related types used throughout Halcyon. */

namespace hal
{
    using pixel_type = lyo::i16;

    using pixel_size = point<pixel_type>;
    using pixel_pos = point<pixel_type>;
    using pixel_area = rectangle<pixel_type>;

    using position_type = lyo::f64;

    using coordinate = point<position_type>;
    using world_area = rectangle<position_type>;

    using opacity_slider = lyo::static_slider<lyo::f64, SDL_ALPHA_TRANSPARENT, SDL_ALPHA_OPAQUE>;
}

// Paranoia.
static_assert(std::is_signed_v<hal::pixel_type>, "Pixel type must be signed");
static_assert(std::is_floating_point_v<hal::position_type>, "Position type must be floating point");

static_assert(sizeof(hal::SDL::point_wrap) == sizeof(SDL_Point) && sizeof(hal::SDL::fpoint_wrap) == sizeof(SDL_FPoint) && sizeof(hal::SDL::rect_wrap) == sizeof(SDL_Rect) && sizeof(hal::SDL::frect_wrap) == sizeof(SDL_FRect), "Incompatible memory layout between SDL structs and their wrappers");