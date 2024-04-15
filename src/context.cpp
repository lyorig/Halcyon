#include <halcyon/debug.hpp>

#include <halcyon/context.hpp>
#include <halcyon/internal/helpers.hpp>

using namespace hal;

context::~context()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}