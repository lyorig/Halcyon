#pragma once

#include <tuple>

// concepts.hpp:
// An extension of STL concepts.

namespace hal
{
    template <typename What, typename... Where>
    constexpr inline bool is_present_v = (std::is_same_v<What, Where> || ...);

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T> || std::is_enum_v<T>;

    template <typename T, typename... Ts>
    concept one_of = is_present_v<T, Ts...>;
}