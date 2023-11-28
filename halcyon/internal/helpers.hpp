#pragma once

#include <bit>
#include <cassert>
#include <halcyon/types/other.hpp>
#include <lyo/concepts.hpp>

namespace hal {
// OR together a set of values.
// The name stands for "initializer list to bit mask".
template <typename Cvt, typename T>
constexpr Cvt il2bm(il<T> list)
{
    Cvt mask { 0 };

    for (const auto value : list) {
        mask |= static_cast<Cvt>(value);
    }

    return mask;
}
}