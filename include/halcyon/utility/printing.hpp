#pragma once

#include <halcyon/types/rectangle.hpp>
#include <ostream>
#include <utility>

// printing.hpp:
// Stream insertion support for custom types.

namespace hal
{
    template <typename T>
    concept printable = requires(std::ostream& str, const T& val) { str << val; };

    // One-byte types get interpreted as characters. This tries to prevent that.
    template <arithmetic T>
    using printable_int_t = std::conditional_t<sizeof(T) == 1, std::conditional_t<std::is_signed_v<T>, i16, u16>, T>;

    // Convenience function.
    template <arithmetic T>
    constexpr printable_int_t<T> to_printable_int(T val)
    {
        return static_cast<printable_int_t<T>>(val);
    }

    // Ensure enums get printed properly.
    template <typename T>
        requires std::is_enum_v<T>
    std::ostream& operator<<(std::ostream& str, T val)
    {
        return str << to_printable_int(std::to_underlying(val));
    }

    template <arithmetic T>
    std::ostream& operator<<(std::ostream& str, const point<T>& pt)
    {
        return str << '[' << to_printable_int(pt.x) << ',' << to_printable_int(pt.y) << ']';
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& str, const rectangle<T>& rect)
    {
        return str << '{' << rect.pos << ' ' << rect.size << '}';
    }

    namespace video
    {
        class display;
    }

    std::ostream& operator<<(std::ostream& std, const video::display& disp);
}