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

    // A type that can be used as a string.
    template <typename T>
    concept string_like = std::is_assignable_v<std::string_view, T>;

    template <typename T>
    concept buffer = requires(T& x) {std::data(x); std::size(x); } && sizeof(*std::data(std::declval<const T&>())) == 1;

    // A type that represents a static/dynamic array.
    // The element type must be 1 byte large.
    template <typename T>
    concept rwops_buffer = !string_like<T> && buffer<T>;
}