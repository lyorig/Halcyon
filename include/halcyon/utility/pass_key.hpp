#pragma once

#include <type_traits>

namespace hal
{
    template <typename T>
    class pass_key
    {
        friend T;

        constexpr pass_key() { }
    };
}