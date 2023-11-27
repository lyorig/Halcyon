#pragma once

#include <cstring>
#include <iomanip>
#include <sstream>

#include "concepts.hpp"

/* utility.hpp:
   Various uncategorized helper functions. */

namespace lyo
{
    template <character T>
    requires(lyo::is_any_of<T, char, wchar_t>()) constexpr bool streq(const T* first, const T* second) noexcept
    {
        if (std::is_same_v<T, char>)
            return std::strcmp(first, second) == 0;

        else if (std::is_same_v<T, char*>)
            return std::wcscmp(first, second) == 0;
    }

    // Checks if the string is null or if the first character is a null terminator.
    template <character T>
    constexpr bool is_c_string_empty(const T* string) noexcept
    {
        return !string || *string == static_cast<T>('\0');
    }

    // Input all arguments into a stringstream and return them as a string.
    template <typename... Args>
    std::string string_from_pack(Args... args) noexcept
    {
        std::stringstream stream;

        stream << std::fixed;

        (stream << ... << args);

        return stream.str();
    }

    template <typename T>
    requires std::is_enum_v<T>
        std::underlying_type_t<T> to_underlying(T value)
    noexcept
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }
}  // namespace lyo