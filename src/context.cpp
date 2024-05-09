#include <halcyon/context.hpp>

#include <csignal>

#include <SDL.h>

#include <halcyon/debug.hpp>

using namespace hal;

void signal_handler(int code)
{
    switch (code)
    {
    case SIGSEGV:
        HAL_PRINT("A segmentation fault has occurred under Halcyon. If this is an error in the library, please complain on GitHub.");
        break;
    case SIGINT:
        HAL_PRINT("Halcyon has been interrupted.");
        break;
    }

    std::exit(EXIT_FAILURE);
}

constexpr int signals[] { SIGSEGV, SIGINT };

context::context()
{
    HAL_PRINT(debug::severity::init, "Initialized Halcyon context");

    // Set signals.
    if constexpr (compile_settings::debug_enabled)
    {
        for (int sig : signals)
            std::signal(sig, signal_handler);
    }
}

context::~context()
{
    // Reset signals.
    if constexpr (compile_settings::debug_enabled)
    {
        for (int sig : signals)
            std::signal(sig, SIG_DFL);
    }

    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}