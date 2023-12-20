#pragma once

#include "concepts.hpp"
#include <cmath>

// cast.hpp:
// Casting with proper rounding.

namespace lyo
{

    // Cast to a type with proper rounding.
    template <arithmetic To, arithmetic From>
    constexpr To cast(From value) noexcept
    {
        if constexpr (std::is_integral_v<To> && std::is_floating_point_v<From>)
        {
            if (std::is_constant_evaluated())
                return static_cast<To>(value < 0.0 ? value - 0.5 : value + 0.5);

            else
                return static_cast<To>(std::round(value));
        }

        else
            return static_cast<To>(value);
    }
} // namespace lyo