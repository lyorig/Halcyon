#pragma once

#include <halcyon/types/render.hpp>

namespace quest
{
    constexpr lyo::f64 gravity { 9.81 };

    // All of the following values apply to the base camera distance and scale.

    // Different aspect ratios exist, and it is my belief that "anchoring"
    // the height instead of the width is the better option. As such, the
    // corresponding width dimensions for these values are calculated at runtime.

    // HalQuest operates on two types of pixel measurements:
    //  - Renderer pixels (rpx)
    //      Pixels of the underlying Halcyon renderer.
    //  - Art pixels (apx)
    //      Pixels of the pixel art used in the game.

    // The height of the "canvas" (renderer).
    constexpr hal::pixel_t canvas_height { 1080 };

    // The canvas height of pixel art.
    constexpr hal::pixel_t apx_canvas_height { 108 };

    constexpr hal::pixel_t apx_per_meter {};

    // How many renderer pixels correspond to one art pixel.
    constexpr hal::pixel_t apx_scale { canvas_height / apx_canvas_height };

    // For faster processing, multiply by ints, not floats.
    static_assert(canvas_height % apx_canvas_height == 0);
}