#pragma once

#include <iomanip>
#include <sstream>

#include <lyo/concepts.hpp>

// strutil.hpp:
// String utility functions.

namespace lyo
{
    // String equality check.
    bool streq(const char* lhs, const char* rhs) noexcept;
    bool streq(const wchar_t* lhs, const wchar_t* rhs) noexcept;

    // Input all arguments into a stringstream and return them as a string.
    template <typename... Args>
    std::string string_from_pack(Args&&... args) noexcept
    {
        // Warning suppression.
        if constexpr (sizeof...(Args) == 0)
            return {};

        else
        {
            std::stringstream stream;

            stream << std::fixed;

            (stream << ... << std::forward<Args>(args));

            return stream.str();
        }
    }
}