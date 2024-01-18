#pragma once

#include <halcyon/types/render.hpp>

// constants.hpp:
// Constexpr stuff, neatly organized in a single file.

namespace quest
{
    // The game distinguishes two types of pixels.
    //  - Renderer pixels (RPx): The pixels of the underlying Halcyon renderer.
    //  - Art pixels (APx):      Pixel-art pixels, multiple times larger than renderer pixels.

    // Everything is set using the height, because it's more consistent.
    // Widths are weird - 4:3, ultrawide, etc., so it's better to "anchor" the height.

    namespace constants
    {
        // The Halcyon renderer's height. Changing this value should not change
        // the relative scale of any visuals, but rather the fluidity of motion.
        constexpr hal::pixel_t render_height { 1080 };

        // How many art pixels fit into the height (default scale).
        // It has a direct impact on the scale of visuals, so it should be
        // set in stone at a reasonable point in developement.
        constexpr hal::pixel_t art_height { 196 };

        // The scale with which to multiply pixel-art graphics at zero distance.
        constexpr lyo::f64 art_scale { lyo::f64(render_height) / art_height };
    }
}
