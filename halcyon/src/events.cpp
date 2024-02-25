#include <halcyon/debug.hpp>
#include <halcyon/events.hpp>

using namespace hal;

bool event::poll()
{
    return ::SDL_PollEvent(&data) == 1;
}

void event::push()
{
    HAL_ASSERT_VITAL(::SDL_PushEvent(&data) >= 0, ::SDL_GetError());
}

bool event::pending()
{
    return ::SDL_PollEvent(nullptr) == 1;
}