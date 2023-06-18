#pragma once

#include <lyo/concepts.hpp>
#include <lyo/types.hpp>

#include "point.hpp"
#include "rectangle.hpp"

/* types.hpp:
   Types used throughout Halcyon. */

namespace hal
{
    using pixel_t    = lyo::i16;
    using position_t = lyo::f64;

    using pixel_size = point<pixel_t>;
    using coordinate = point<position_t>;

    using pixel_area = rectangle<pixel_t>;
    using world_area = rectangle<position_t>;

    using color_t = lyo::u32;

    template <typename... Params>
    using callback = lyo::func_ptr<void, Params...>;
}  // namespace hal