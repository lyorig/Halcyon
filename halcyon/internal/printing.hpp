#pragma once

#include <halcyon/types/rectangle.hpp>
#include <ostream>

// printing.hpp:
// Stream insertion support for custom types.

namespace hal
{
    // (u)int8_t types get interpreted as characters. This tries to prevent that.
    template <lyo::arithmetic T>
    using to_printable_int_t = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, lyo::i16, lyo::u16>, T>;

    // Convenience function.
    template <lyo::arithmetic T>
    constexpr to_printable_int_t<T> make_printable_int(T val)
    {
        return to_printable_int_t<T>(val);
    }

    class display;

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, const point<T>& pt)
    {
        return str << '[' << make_printable_int(pt.x) << ',' << make_printable_int(pt.y) << ']';
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& str, const rectangle<T>& rect)
    {
        return str << '{' << rect.pos << ' ' << rect.size << '}';
    }

    std::ostream& operator<<(std::ostream& std, const display& disp);
}
