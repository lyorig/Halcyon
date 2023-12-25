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
    using to_printable_int = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, lyo::i16, lyo::u16>, T>;

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, point<T> pt)
    {
        using print_type = to_printable_int<T>;

        return str << '[' << print_type(pt.x) << ',' << print_type(pt.y) << ']';
    }

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, rectangle<T> rect)
    {
        return str << '{' << rect.pos << ' ' << rect.size << '}';
    }
}