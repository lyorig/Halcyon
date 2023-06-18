#pragma once

#include <lyo/types.hpp>

namespace hal
{
    using color_type = lyo::u32;

    enum color : color_type
    {
        black = 0x000000,

        red   = 0xFF0000,
        green = 0x00FF00,
        blue  = 0x0000FF,

        orange = 0xFFAC1C,

        white = 0xFFFFFF
    };
}  // namespace hal