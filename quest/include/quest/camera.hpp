#pragma once

#include <halcyon/renderer.hpp>
#include <quest/types.hpp>

namespace quest
{
    class camera
    {
    public:
        coord    pos;
        lyo::f64 scale;

        // Transform an object's position to rendering coordinates.
        // The renderer height parameter is used because the coordinate system used under
        // the hood has the origin at the top-left corner, meaning that the height goes "downward."
        hal::coord_point transform(coord obj_pos, meter_t obj_height, hal::pixel_t renderer_height) const;
    };
}
