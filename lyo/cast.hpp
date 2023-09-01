#pragma once

#include <type_traits>

/* cast.hpp:
   Templated casting, correctly rounds floats as well. */

namespace lyo
{
    template <typename Cast_to, typename T>
    T round_cast(T value) noexcept
    {
        if constexpr (std::is_floating_point_v<T> && std::is_integral_v<Cast_to>)
            return static_cast<Cast_to>(value + static_cast<T>(0.5));

        else
            return static_cast<Cast_to>(value);
    }
}