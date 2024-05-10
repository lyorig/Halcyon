#pragma once

#include <initializer_list>
#include <utility>

#include <halcyon/utility/concepts.hpp>

// helpers.hpp:
// One-off functions that I didn't know where else to put.

namespace hal::detail
{
    // OR together a set of values.
    // The name stands for "initializer list to bit mask".
    template <meta::arithmetic Cvt, typename T>
        requires std::is_enum_v<T>
    constexpr Cvt to_bitmask(std::initializer_list<T> list)
    {
        Cvt mask { 0 };

        for (const auto value : list)
            mask |= std::to_underlying(value);

        return mask;
    }
}