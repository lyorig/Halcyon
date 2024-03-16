#pragma once

#include <halcyon/other/system.hpp>

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
        // Create the cleanup class without initializing anything.
        cleanup() = default;

        // Create the cleanup class and initialize some subsystems
        // while we're at it.
        cleanup(std::initializer_list<system::type> subs);

        ~cleanup();
    };
}