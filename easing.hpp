#pragma once

#include <halcyon/types/render.hpp>

// easing.hpp:
// Easing functions for animations etc.

namespace hq
{
    namespace ease
    {
        // Bezier ease-out (control points [0,1], [1,1]).
        constexpr lyo::f64 out(hal::coord_type x)
        {
            return x * (x * x - 3 * x + 3);
        };

        // Bezier ease-in (control points [0,0], [0,1]).
        constexpr lyo::f64 in(hal::coord_type x)
        {
            return x * x * x;
        };

        // Quadratic ease-in-out.
        constexpr lyo::f64 quad(hal::coord_type x)
        {
            lyo::f64 sqr = x * x;
            return sqr / (2.0 * (sqr - x) + 1.0);
        };
    }
}