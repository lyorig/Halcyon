#pragma once

#include <type_traits>

/* concepts.hpp:
   An extension of STL concepts. */

namespace lyo
{
    template <typename What, typename... Where>
    constexpr inline bool is_present_v = (std::is_same_v<What, Where> || ...);

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template <typename T, typename... Ts>
    concept one_of = is_present_v<T, Ts...>;

    template <typename T>
    concept character = one_of<T, char, signed char, unsigned char, wchar_t,
        char8_t, char16_t, char32_t>;

    template <typename T, typename... Ts>
    struct index;

    template <typename T, typename... Ts>
    struct index<T, T, Ts...> : std::integral_constant<std::size_t, 0>
    {
    };

    template <typename T, typename U, typename... Ts>
    struct index<T, U, Ts...> : std::integral_constant<std::size_t, 1 + index<T, Ts...>::value>
    {
    };

    template <typename T, typename... Ts>
    constexpr inline std::size_t index_v = index<T, Ts...>::value;
} // namespace lyo