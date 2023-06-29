#pragma once

#include <lyo/types.hpp>

namespace hal
{
    template <typename... Params>
    using callback = lyo::func_ptr<void, Params...>;
}  // namespace hal