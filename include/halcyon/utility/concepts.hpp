#pragma once

#include <halcyon/utility/metaprogramming.hpp>

// concepts.hpp:
// An extension of STL concepts.

namespace hal::meta
{
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T> || std::is_enum_v<T>;

    template <typename T, typename... Ts>
    concept one_of = is_present_v<T, Ts...>;
}