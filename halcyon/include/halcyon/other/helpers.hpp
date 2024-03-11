#pragma once

#include <initializer_list>
#include <lyo/concepts.hpp>

// helpers.hpp:
// One-off functions that I didn't know where to put.

namespace hal
{
    // OR together a set of values.
    // The name stands for "initializer list to bit mask".
    template <typename Cvt, typename T>
    constexpr Cvt il2bm(std::initializer_list<T> list)
    {
        Cvt mask { 0 };

        for (const auto value : list)
            mask |= Cvt(value);

        return mask;
    }
}