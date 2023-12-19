#pragma once

#include "concepts.hpp"

/* cast.hpp:
   Modular casting. */

namespace lyo
{

    // Cast to a type with proper rounding.
    template <arithmetic To, arithmetic From>
    constexpr To cast(From value) noexcept
    {
        if constexpr (std::is_integral_v<To> && std::is_floating_point_v<From>)
        {
            constexpr From tolerance { 1.0 - 0.5 }; // RHS = where to round up from.
            return static_cast<To>(value < 0.0 ? value - tolerance : value + tolerance);
        }

        else
            return static_cast<To>(value);
    }
} // namespace lyo