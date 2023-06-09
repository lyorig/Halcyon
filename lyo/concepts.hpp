#pragma once

#include <type_traits>

/* concepts.hpp:
   An extension of STL concepts. */

namespace lyo
{
    template <typename T, typename... Pack>
    constexpr bool is_any_of()
    {
        return (std::is_same_v<T, Pack> || ...);
    }

    template <typename T>
    concept arithmetic = std::is_arithmetic_v<T>;

    template <typename T>
    concept character = is_any_of<T, char, signed char, unsigned char, wchar_t, char8_t, char16_t, char32_t>();
}  // namespace lyo