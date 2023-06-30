#pragma once

#include <lyo/types.hpp>

namespace hal
{
    // Keep this signed, as SDL_mixer uses a -1 value as an infinite loop.
    using loop_type = lyo::i16;

    // Loop music/SFX as long as possible (~65K times).
    constexpr loop_type infinite_loop { -1 };

}  // namespace hal

// I'm paranoid.
static_assert(std::is_signed_v<hal::loop_type>, "Loop type must be signed");