#pragma once

#include <initializer_list>

#include <lyo/types.hpp>

#include <SDL.h>

// cleanup.hpp:
// A RAII class that manages SDL (de)initialization.

namespace hal
{
    enum class system : lyo::u8
    {
        video = SDL_INIT_VIDEO
    };

    // This class initializes everything Halcyon needs.
    // Its creation might not be necessary for all functionality, but it's good
    // practice to have it initialized before using anything else regardless.
    class cleanup
    {
    public:
        // Initialize the SDL base and nothing else.
        cleanup();

        // Initialize SDL along with some subsystems.
        cleanup(std::initializer_list<system> subs);

        ~cleanup();
    };
}