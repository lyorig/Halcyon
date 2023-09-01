#pragma once

#include <halcyon/types/color.hpp>

namespace hal
{
    enum class severity : rgb::hex_type
    {
        info    = static_cast<rgb::hex_type>(color::white),
        warning = static_cast<rgb::hex_type>(color::orange),
        error   = static_cast<rgb::hex_type>(color::red),

        init = static_cast<rgb::hex_type>(color::green),
        load = static_cast<rgb::hex_type>(color::cyan)
    };
}