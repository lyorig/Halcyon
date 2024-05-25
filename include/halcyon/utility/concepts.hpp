#pragma once

#include <halcyon/utility/metaprogramming.hpp>
#include <string_view>

// concepts.hpp:
// An extension of STL concepts.

namespace hal::meta
{
    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T> || std::is_enum_v<T>;

    template <typename T, typename... Ts>
    concept one_of = is_present<T, Ts...>;

    template <typename T>
    concept bare = is_bare<T>;

    template <typename T>
    concept buffer = requires(T& x) {std::data(x); std::size(x); } && sizeof(*std::data(std::declval<const T&>())) == 1;
}