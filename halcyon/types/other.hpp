#pragma once

#include <initializer_list>
#include <lyo/types.hpp>

namespace hal
{
    template <typename... Params>
    using callback = lyo::func_ptr<void, Params...>;

    template <typename Type>
    using il = std::initializer_list<Type>;
}