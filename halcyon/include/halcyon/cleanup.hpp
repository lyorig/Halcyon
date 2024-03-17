#pragma once

#include <halcyon/other/system.hpp>
#include <initializer_list>

// cleanup.hpp:
// A class that deinitializes SDL upon destruction.

namespace hal
{
    // This class deinitializes everything SDL-related upon destruction.
    // You don't have to create it, but it's probably good practice to
    // create one before using anything else from Halcyon.
    class cleanup
    {
    public:
        // Initialize the SDL base and nothing else.
        cleanup();

        // Initialize SDL along with some subsystems.
        cleanup(std::initializer_list<system::type> subs);

        ~cleanup();
    };
}