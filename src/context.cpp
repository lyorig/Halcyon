#include <halcyon/context.hpp>

#include <SDL.h>

#include <halcyon/debug.hpp>

using namespace hal;

std::string_view hal::platform()
{
    return ::SDL_GetPlatform();
}

context::context()
{
    HAL_PRINT(debug::severity::init, "Initialized Halcyon context, running on ", platform());
}

context::~context()
{
    ::SDL_Quit();
    HAL_PRINT("Destroyed Halcyon context. Last error: ", debug::last_error());
}