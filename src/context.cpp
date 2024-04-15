#include <halcyon/debug.hpp>

#include <SDL.h>

#include <halcyon/context.hpp>

using namespace hal;

context::~context()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}