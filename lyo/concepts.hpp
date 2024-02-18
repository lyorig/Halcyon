#pragma once

#include <tuple>
/* concepts.hpp:
   An extension of STL concepts. */

namespace lyo
{
    template <typename What, typename... Where>
    constexpr inline bool is_present_v = (std::is_same_v<What, Where> || ...);

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T> || std::is_enum_v<T>;

    template <typename T, typename... Ts>
    concept one_of = is_present_v<T, Ts...>;

    template <typename T>
    concept character = one_of<T, char, signed char, unsigned char, wchar_t,
        char8_t, char16_t, char32_t>;

    template <typename From, typename To>
    concept static_castable = requires(From val) { static_cast<To>(val); };

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

    template <std::size_t Index, typename... Args>
    using type_at_index = std::tuple_element_t<Index, std::tuple<Args...>>;
}