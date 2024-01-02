#pragma once

#include <halcyon/enums/buttons.hpp>
#include <halcyon/types/packed_array.hpp>
#include <halcyon/types/rectangle.hpp>
#include <ostream>
#include <utility>

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

    // Ensure enums get printed properly.
    template <typename T>
        requires std::is_enum_v<T>
    std::ostream& operator<<(std::ostream& str, T val)
    {
        return str << std::to_underlying(val);
    }

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

    template <typename T, std::size_t Size, T Empty_Value>
    std::ostream& operator<<(std::ostream& str, const packed_array<T, Size, Empty_Value>& arr)
    {
        str << "[ ";

        for (const T value : arr)
            str << value << ' ';

        return str << ']';
    }

    class display;
    std::ostream& operator<<(std::ostream& std, const display& disp);
}
