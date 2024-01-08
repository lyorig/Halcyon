#pragma once

#include <halcyon/types/render.hpp>

// easing.hpp:
// Easing functions for animations etc.

namespace quest
{
    namespace bezier
    {
        // Linear curve (y = x).
        template <std::floating_point T>
        constexpr T linear(T x)
        {
            return x;
        }

        // Bezier ease-out (y = x^3 - 3x^2 + 3x).
        template <std::floating_point T>
        constexpr T ease_out(T x)
        {
            return x * (x * (x - T(3)) + T(3));
        };

        // Bezier ease-in (y = x^3).
        template <std::floating_point T>
        constexpr T ease_in(T x)
        {
            return x * x * x;
        };

        // Quadratic ease-in-out (weird formula, google it).
        template <std::floating_point T>
        constexpr T quadratic(T x)
        {
            const T sqr { x * x };
            return sqr / (T(2.0) * (sqr - x) + T(1.0));
        };
    }
}