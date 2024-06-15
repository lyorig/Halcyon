#pragma once

#include <ostream>

#include <halcyon/types/numeric.hpp>
#include <halcyon/utility/concepts.hpp>

// printing.hpp:
// Extra utility stuff for printing.

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

    namespace meta
    {
        template <typename T>
        concept printable = requires(std::ostream& str, const T& val) { str << val; };
    }

    std::string_view to_string();

    template <typename T>
        requires requires { hal::to_string(std::declval<const T&>()); }
    std::ostream& operator<<(std::ostream& str, const T& obj)
    {
        return str << hal::to_string(obj);
    }

    namespace event
    {
        template <typename T>
            requires requires { hal::to_string(std::declval<const T&>()); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }

    namespace audio
    {
        template <typename T>
            requires requires { hal::to_string(std::declval<const T&>()); }
        std::ostream& operator<<(std::ostream& str, const T& obj)
        {
            return str << hal::to_string(obj);
        }
    }
}