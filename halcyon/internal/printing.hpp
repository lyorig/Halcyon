#pragma once

#include <halcyon/enums/buttons.hpp>
#include <halcyon/types/packed_array.hpp>
#include <halcyon/types/rectangle.hpp>
#include <halcyon/types/sparse_array.hpp>
#include <ostream>
#include <utility>

// printing.hpp:
// Stream insertion support for custom types.

namespace hal
{
    // (u)int8_t types get interpreted as characters. This tries to prevent that.
    template <lyo::arithmetic T>
    using printable_int_t = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, lyo::i16, lyo::u16>, T>;

    // Convenience function.
    template <lyo::arithmetic T>
    constexpr printable_int_t<T> to_printable(T val)
    {
        return printable_int_t<T>(val);
    }

    // Ensure enums get printed properly.
    template <typename T>
        requires std::is_enum_v<T>
    std::ostream& operator<<(std::ostream& str, T val)
    {
        return str << to_printable(std::to_underlying(val));
    }

    template <lyo::arithmetic T>
    std::ostream& operator<<(std::ostream& str, const point<T>& pt)
    {
        return str << '[' << to_printable(pt.x) << ',' << to_printable(pt.y) << ']';
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

    template <typename T, std::size_t Size, T Empty_Value>
    std::ostream& operator<<(std::ostream& str, const sparse_array<T, Size, Empty_Value>& arr)
    {
        str << "[ ";

        for (const T value : arr)
            str << value << ' ';

        return str << ']';
    }

    class display;
    std::ostream& operator<<(std::ostream& std, const display& disp);
}