#pragma once

#include <SDL.h>
#include <initializer_list>
#include <lyo/types.hpp>

// engine.hpp:
// The core of Halcyon, without which nothing can be initialized.

namespace hal
{
    namespace system
    {
        enum type : lyo::u16
        {
            video      = SDL_INIT_VIDEO,
            audio      = SDL_INIT_AUDIO,
            controller = SDL_INIT_GAMECONTROLLER,
            haptic     = SDL_INIT_HAPTIC,
            sensor     = SDL_INIT_SENSOR,
            all        = SDL_INIT_EVERYTHING
        };

        void initialize(type s);
        void deinitialize(type s);

        bool initialized(type s);
    }
}