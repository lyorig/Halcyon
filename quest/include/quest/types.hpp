#pragma once

#include <halcyon/types/point.hpp>

namespace quest
{
    // An object's position in the world. Guess in what unit!
    using meter_t = lyo::f32;

    // The time elapsed between frames.
    using delta_t = lyo::f64;

    // An object's mass, in kilos.
    using mass_t = lyo::u16;

    using coord = hal::point<meter_t>;

    enum direction : lyo::i8
    {
        left  = -1,
        right = 1
    };
}