#pragma once

#include <string_view>

// context.hpp:
// A RAII class that ensures Halcyon deinitialization.

#define HAL_DETAIL_STRINGIFY(x)        #x
#define HAL_DETAIL_STRINGIFY_EXPAND(x) HAL_DETAIL_STRINGIFY(x)

namespace hal
{
    // Get the name of the current platform.
    std::string_view platform();

    namespace compile_settings
    {
        // Check whether SDL has redefined "main" to be its own SDL_main. Use hal::meta::is_correct_main to check.
        constexpr bool main_redefined { std::string_view { HAL_DETAIL_STRINGIFY_EXPAND(main) } != "main" };
    }

    // The base context of Halcyon.
    // This class needs to be created first in order to initialize
    // subsystems. Its destruction means everything gets unloaded.
    class context
    {
    public:
        // Initialize Halcyon with no subsystems.
        context();

        context(const context&) = delete;
        context(context&&)      = delete;

        // Forcefully shuts down everything.
        ~context();
    };
}