#pragma once

#include <lyo/types.hpp>

/* config.hpp:
   Compilation settings for Halcyon. */

namespace hal
{
    namespace cfg
    {
        constexpr bool performance_mode           = true;
        constexpr bool subpixel_drawing_precision = true;

        constexpr lyo::u8 max_console_entries = 8;
    }  // namespace cfg
}  // namespace hal