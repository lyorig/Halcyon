#pragma once

#include <initializer_list>
#include <lyo/concepts.hpp>

// helpers.hpp:
// One-off functions that I didn't know where else to put.

namespace hal
{
    // OR together a set of values.
    // The name stands for "initializer list to bit mask".
    template <lyo::arithmetic Cvt, std::convertible_to<Cvt> T>
    constexpr Cvt il2bm(std::initializer_list<T> list)
    {
        Cvt mask { 0 };

        for (const auto value : list)
            mask |= static_cast<Cvt>(value);

        return mask;
    }
}