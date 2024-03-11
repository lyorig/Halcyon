#pragma once

#include <halcyon/types/render.hpp>

#include <quest/types.hpp>

namespace quest
{
    namespace constants
    {
        constexpr meter_t gravity { 9.81 };

        // All of the following values apply to the base camera distance and scale.

        // Different aspect ratios exist, and it is my belief that "anchoring"
        // the height instead of the width is the better option. As such, the
        // corresponding width dimensions for these values are calculated at runtime.

        // HalQuest operates on two types of pixel measurements:
        //  - Renderer pixels (rpx)
        //      Pixels of the underlying Halcyon renderer.
        //  - Art pixels (apx)
        //      Pixels of the pixel art used in the game.

        constexpr hal::pixel_t
            // The height of the "canvas" (renderer). This setting should only affect the fluidity of
            // motion, and not the actual perceived scale of anything.
            rpx_height { 1024 },
            // How many art pixels constitute a meter in-game. This will affect scaling, so it's best to
            // set this value early in developement and then forget about it.
            apx_per_meter { 6 },
            apx_height { 128 };

        // How many renderer pixels correspond to one art pixel.
        constexpr meter_t apx_scale { static_cast<meter_t>(rpx_height) / apx_height };

        constexpr hal::pixel_point rpx_size(hal::pixel_point actual_size)
        {
            return actual_size * (rpx_height / lyo::f64(actual_size.y));
        }
    }
}