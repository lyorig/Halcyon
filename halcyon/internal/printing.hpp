#pragma once

#include <halcyon/types/rectangle.hpp>
#include <iostream>
#include <lyo/concepts.hpp>

// printing.hpp:
// Stream insertion support for custom types.

namespace hal
{
    // (u)int8_t types get interpreted as characters. This tries to prevent that.
    template <lyo::arithmetic T>
    using to_printable_int_t = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, lyo::i16, lyo::u16>, T>;

    // Convenience function.
    template <lyo::arithmetic T>
    constexpr to_printable_int_t<T> make_printable(T val)
    {
        return to_printable_int_t<T>(val);
    }

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, point<T> pt)
    {
        return str << '[' << make_printable(pt.x) << ',' << make_printable(pt.y) << ']';
    }

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, rectangle<T> rect)
    {
        return str << '{' << rect.pos << ' ' << rect.size << '}';
    }
}