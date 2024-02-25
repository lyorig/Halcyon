#pragma once

#include <SDL_events.h>
#include <halcyon/enums/buttons.hpp>

// events.hpp:
// A thin wrapper of SDL's event handling system.
// As there are many various events that would be quite time-consuming to adapt
// (or am I just lazy?), a zero-overhead wrapper is provided instead.

namespace hal
{
    class event
    {
    public:
        enum type
        {
            key_press   = SDL_KEYDOWN,
            key_release = SDL_KEYUP,

            quit_request = SDL_QUIT
        };

        // Get an event from the event queue.
        bool poll();

        // Push this event into the event queue.
        void push();

        // Check whether there are any pending events in the event queue.
        static bool pending();

        SDL_Event data;
    };
}