#pragma once

#include <halcyon/types/color.hpp>
#include <utility>

namespace hal
{
    enum class severity : color::hex_type
    {
        info = std::to_underlying(color::white),
        warning = std::to_underlying(color::orange),
        error = std::to_underlying(color::red),

        init = std::to_underlying(color::green),
        load = std::to_underlying(color::cyan)
    };
}