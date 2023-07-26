#pragma once

#include <iomanip>
#include <sstream>

#include "concepts.hpp"

/* utility.hpp:
   Various uncategorized helper functions. */

namespace lyo
{
    // Checks if the first character of the string is a null terminator.
    template <character T>
    bool is_c_string_empty(const T* string) noexcept
    {
        return *string == static_cast<T>('\0');
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
}  // namespace lyo