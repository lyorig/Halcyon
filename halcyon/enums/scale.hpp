#pragma once

#include <halcyon/types/point.hpp>

namespace hal
{
    struct scale
    {
        enum mode
        {
            x,
            y
        };

        template <lyo::arithmetic T>
        constexpr static lyo::f64 resolve(mode m, const point<T>& source, const point<T>& dest, lyo::f64 desired_percentage)
        {
            if (m == x)
            {
                return (dest.x * desired_percentage) / source.x;
            }

            else
            {
                return (dest.y * desired_percentage) / source.y;
            }
        }
    };
}
