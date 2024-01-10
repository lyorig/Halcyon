#pragma once

#include <halcyon/types/point.hpp>
#include <lyoSTL/concepts.hpp>

namespace hal
{
    struct scale
    {
        enum type
        {
            x,
            y,
            fractional
        };

        template <lyo::arithmetic T>
        constexpr static point<T> resolve(type scale_type, point<T> src, lyo::f64 desired)
        {
            lyo::f64 scale;

            switch (scale_type)
            {
            case x:
                scale = desired / src.x;

            case y:
                scale = desired / src.y;

            case fractional:
                scale = desired;
            }

            return src * scale;
        }
    };
}