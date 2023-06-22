#pragma once

#include <lyo/types.hpp>

#include "point.hpp"
#include "rectangle.hpp"

/* render.hpp:
   Rendering-related types used throughout Halcyon. */

namespace hal
{
    using pixel_t    = lyo::i16;
    using position_t = lyo::f64;

    using pixel_size = point<pixel_t>;

    using pixel_coord = point<pixel_t>;
    using coordinate  = point<position_t>;

    using pixel_area = rectangle<pixel_t>;
    using world_area = rectangle<position_t>;
}  // namespace hal