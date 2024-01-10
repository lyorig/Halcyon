#pragma once

#include <concepts>

namespace lyo
{
    template <std::integral T>
    constexpr auto factorial(T n) -> decltype(n * n)
    {
        T res { 1 };
        for (T i { 1 }; i <= n; ++i)
            res *= i;
        return res;
    };
}