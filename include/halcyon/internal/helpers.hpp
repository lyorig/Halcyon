#pragma once

#include <initializer_list>

#include <halcyon/utility/concepts.hpp>

// helpers.hpp:
// One-off functions that I didn't know where else to put.

namespace hal::detail
{
    // OR together a set of values.
    // The name stands for "initializer list to bit mask".
    template <meta::arithmetic To, typename T>
        requires std::is_enum_v<T>
    constexpr To to_bitmask(std::initializer_list<T> list)
    {
        To mask { 0 };

        for (const auto value : list)
            mask |= static_cast<To>(value);

        return mask;
    }
}