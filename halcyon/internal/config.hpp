#pragma once

#include <lyo/types.hpp>

/* config.hpp:
   Compilation settings for Halcyon. */

namespace hal {
namespace cfg {
    constexpr bool performance_mode = true; // Enables theoretically faster, potentially risky code.

    constexpr lyo::u8 max_console_entries = 8; // The max amount of console entries before they start getting popped off the queue.
} // namespace cfg
} // namespace hal