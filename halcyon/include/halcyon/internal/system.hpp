#pragma once

#include <SDL.h>
#include <lyo/types.hpp>

// system.hpp:
// Subsystem handling.

namespace hal
{
    namespace system
    {
        // SDL subsystem types. Right now, the only implemented one is video.
        enum type : lyo::u16
        {
            video = SDL_INIT_VIDEO
        };

        void initialize(type s);
        void deinitialize(type s);

        bool initialized(type s);
    }
}