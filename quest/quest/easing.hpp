#pragma once

#include <cassert>
#include <halcyon/types/render.hpp>

// easing.hpp:
// Easing functions for animations etc.

namespace quest
{
    namespace bezier
    {
        // Define your own Bezier curve. Implicitly, y0 == 0 and y3 == 1.
        constexpr lyo::f64 curve(lyo::f64 y1, lyo::f64 y2, lyo::f64 t)
        {
            const lyo::f64 par { 1.0 - t }; // (1 - t)
            return 3.0 * std::pow(par, 2) * t * y1 + 3.0 * par * std::pow(t, 2) * y2 + std::pow(t, 3);
        }

        // y = x
        constexpr lyo::f64 linear(lyo::f64 t)
        {
            return t;
        }

        // y = x^3 - 3x^2 + 3x
        constexpr lyo::f64 ease_out(lyo::f64 t)
        {
            return curve(1.0, 1.0, t);
        };

        // y = x^3
        constexpr lyo::f64 ease_in(lyo::f64 t)
        {
            return curve(0.0, 1.0, t);
        };

        // Quadratic ease-in-out (weird formula, google it).
        constexpr lyo::f64 quadratic(lyo::f64 t)
        {
            const lyo::f64 sqr { t * t };
            return sqr / (2.0 * (sqr - t) + 1.0);
        };
    }
}