#pragma once

#include <halcyon/types/rectangle.hpp>
#include <ostream>

// printing.hpp:
// Stream insertion support for custom types.

namespace hal
{
    // One-byte types get interpreted as characters. This tries to prevent that.
    template <meta::arithmetic T>
    using printable_int_t = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, i16, u16>, T>;

    // Ensure a number is printable; that is, if 'val' is of a type tbat
    // would cause it to be interpreted as a character by output streams,
    // it'll be widened to the next smallest integer type.
    template <meta::arithmetic T>
    constexpr printable_int_t<T> to_printable_int(T val)
    {
        return static_cast<printable_int_t<T>>(val);
    }

    template <meta::arithmetic T>
    std::ostream& operator<<(std::ostream& str, const point<T>& pt)
    {
        return str << '[' << to_printable_int(pt.x) << ',' << to_printable_int(pt.y) << ']';
    }

    template <meta::arithmetic T>
    std::ostream& operator<<(std::ostream& str, const rectangle<T>& rect)
    {
        return str << '{' << rect.pos << ' ' << rect.size << '}';
    }

    struct color;

    namespace info
    {
        class display;
        class renderer;
    }

    std::ostream& operator<<(std::ostream& str, color c);
    std::ostream& operator<<(std::ostream& str, const info::display& disp);
    std::ostream& operator<<(std::ostream& str, const info::renderer& inf);

    namespace meta
    {
        template <typename T>
        concept printable = requires(std::ostream& str, const T& val) { str << val; };
    }
}