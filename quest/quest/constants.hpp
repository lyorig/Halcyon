#pragma once

#include <halcyon/types/render.hpp>

// constants.hpp:
// Constexpr stuff, neatly organized in a single file.

namespace quest
{
    // HalQ distinguishes two types of pixels.
    //  - Renderer pixels (RPx): The pixels of the underlying Halcyon renderer.
    //  - Art pixels (APx):      Pixel-art pixels, multiple times larger than renderer pixels.

    namespace constants
    {
        // The Halcyon renderer's height.
        // The width is calculated upon startup.
        constexpr hal::pixel_t render_height { 1080 };

        // How many art pixels fit into the height (default scale).
        // The width is calculated upon startup.
        constexpr hal::pixel_t apx_height { 128 };

        constexpr lyo::f64 apx_scale { lyo::f64(render_height) / apx_height };
    }
}
