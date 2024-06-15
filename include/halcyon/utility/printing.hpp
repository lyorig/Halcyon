#pragma once

#include <ostream>

#include <halcyon/types/numeric.hpp>
#include <halcyon/utility/concepts.hpp>

// printing.hpp:
// Extra utility stuff for printing.

namespace hal
{
    namespace meta
    {
        // One-byte types get interpreted as characters. This tries to prevent that.
        template <arithmetic T>
        using make_printable_int = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, i16, u16>, T>;
    }

    // Ensure a number is printable; that is, if 'val' is of a type tbat
    // would cause it to be interpreted as a character by output streams,
    // it'll be widened to the next smallest integer type.
    template <meta::arithmetic T>
    constexpr meta::make_printable_int<T> to_printable_int(T val)
    {
        return static_cast<meta::make_printable_int<T>>(val);
    }

    namespace meta
    {
        template <typename T>
        concept printable = requires(std::ostream& str, const T& val) { str << val; };
    }
}