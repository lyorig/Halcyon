#pragma once

#include <string_view>

#include <SDL.h>

#include <halcyon/debug.hpp>
#include <halcyon/internal/subsystem.hpp>
#include <lyo/pass_key.hpp>

// context.hpp:
// A RAII class that manages Halcyon (de)initialization.

namespace hal
{
    // The base context of Halcyon.
    class context
    {
    public:
        // Initialize Halcyon with no subsystems.
        context() = default;

        // Forcefully shuts down everything.
        ~context();

    private:
        // Delegating constructor.
        context(std::uint32_t flags);
    };
}