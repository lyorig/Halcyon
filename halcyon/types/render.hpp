#pragma once

#include <lyo/types.hpp>

#include "point.hpp"
#include "rectangle.hpp"

/* render.hpp:
   Rendering-related types used throughout Halcyon. */

namespace hal
{
    using pixel_type = lyo::i16;

    using pixel_size = point<pixel_type>;
    using pixel_pos  = point<pixel_type>;
    using pixel_area = rectangle<pixel_type>;

    using position_type = lyo::f32;

    using coordinate = point<position_type>;
    using world_area = rectangle<position_type>;
}

// Paranoia.
static_assert(std::is_signed_v<hal::pixel_type>, "Pixel type must be signed");