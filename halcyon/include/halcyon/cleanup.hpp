#pragma once

#include <halcyon/other/system.hpp>
#include <initializer_list>

// cleanup.hpp:
// A class that manages SDL (de)initialization.

namespace hal
{
    // This class initializes everything Halcyon needs.
    // Its creation might not be necessary for all functionality, but in
    // case of
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