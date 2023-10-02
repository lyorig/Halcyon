#pragma once

#include <lyo/types.hpp>
#include <initializer_list>

namespace hal
{
template <typename... Params>
using callback = lyo::func_ptr<void, Params...>;

template <typename Type>
using il = std::initializer_list<Type>;
}