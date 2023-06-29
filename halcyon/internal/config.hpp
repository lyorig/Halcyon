#pragma once

#include <lyo/types.hpp>

/* config.hpp:
   Compilation settings for Halcyon. */

namespace hal
{
    namespace cfg
    {
        constexpr bool performance_mode           = true;  // Currently only used in events::binder. Potentially unstable.
        constexpr bool subpixel_drawing_precision = true;  // Whether to use SDL's floating-point RenderCopy function.

        constexpr lyo::u8 max_console_entries = 10;  // The max amount of console entries before they start getting popped off the queue.
    }  // namespace cfg
}  // namespace hal