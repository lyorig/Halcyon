#pragma once

#include "colors.hpp"

namespace hal
{
    enum class severity : color_type
    {
        info    = static_cast<color_type>(color::white),
        warning = static_cast<color_type>(color::orange),
        error   = static_cast<color_type>(color::red),

        init = static_cast<color_type>(color::green),
        load = static_cast<color_type>(color::cyan)
    };
}