#pragma once

#include <halcyon/renderer.hpp>
#include <quest/types.hpp>

namespace quest
{
    // A camera that transforms object positions into rending coordinates.
    // The position represents the center of the viewport, not a corner.
    class camera
    {
    public:
        coord pos;

        // Transform an object's position to rendering coordinates.
        // The renderer height parameter is used because the coordinate system used under
        // the hood has the origin at the top-left corner, meaning that the height goes "downward."
        hitbox transform(hitbox hbx, hal::pixel_point rsz) const;
    };
}
