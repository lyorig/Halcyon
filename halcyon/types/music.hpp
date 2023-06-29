#pragma once

#include <lyo/types.hpp>

namespace hal
{
    // Keep this signed, as SDL_mixer uses a -1 value as an infinite loop.
    using loop_type = lyo::i16;
}