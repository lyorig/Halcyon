#pragma once

// context.hpp:
// A RAII class that ensures Halcyon deinitialization.

namespace hal
{
    // The base context of Halcyon.
    // This class needs to be created first in order to initialize
    // subsystems. Its destruction means everything gets unloaded.
    class context
    {
    public:
        // Initialize Halcyon with no subsystems.
        context() = default;

        // Forcefully shuts down everything.
        ~context();
    };
}