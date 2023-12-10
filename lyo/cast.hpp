#pragma once

#include "concepts.hpp"
#include <cmath>

/* cast.hpp:
   Modular casting. */

namespace lyo {

// For now, this does nothing. Float rounding will be
// implemented when I consider it important.
template <arithmetic Cast_to, arithmetic T>
constexpr Cast_to round_cast(T value) noexcept
{
    return static_cast<Cast_to>(std::round(value));
}
} // namespace lyo