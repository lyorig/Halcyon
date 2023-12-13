#pragma once

#include <halcyon/types/color.hpp>

namespace hal
{
    enum class severity : color::hex_type
    {
        info = static_cast<color::hex_type>(color::white),
        warning = static_cast<color::hex_type>(color::orange),
        error = static_cast<color::hex_type>(color::red),

        init = static_cast<color::hex_type>(color::green),
        load = static_cast<color::hex_type>(color::cyan)
    };
}