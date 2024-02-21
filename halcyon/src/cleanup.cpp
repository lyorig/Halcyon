#include <halcyon/cleanup.hpp>
#include <halcyon/debug.hpp>

using namespace hal;

cleanup::cleanup(std::initializer_list<system::type> subs)
{
    for (auto s : subs)
        initialize(s);
}

cleanup::~cleanup()
{
    HAL_PRINT("Cleaning up SDL. Last error: ", debug::last_error());
    ::SDL_Quit();
}