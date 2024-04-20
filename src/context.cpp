#include <halcyon/debug.hpp>

#include <SDL3/SDL.h>

#include <halcyon/context.hpp>

using namespace hal;

context::context()
{
    HAL_PRINT(debug::severity::init, "Initialized Halcyon context");
}

context::~context()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}