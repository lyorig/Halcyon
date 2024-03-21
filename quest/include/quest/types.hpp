#pragma once

#include <halcyon/types/point.hpp>

namespace quest
{
    // An object's position in the world. Guess in what unit!
    using meter_t = lyo::f32;

    // The time elapsed between frames.
    using delta_t = lyo::f64;

    using coord  = hal::point<meter_t>;
    using hitbox = hal::rectangle<meter_t>;

    enum direction : lyo::i8
    {
        right = 1,
        left  = -1,

        up   = 1,
        down = -1
    };
}