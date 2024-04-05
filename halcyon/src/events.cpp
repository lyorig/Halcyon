#include <halcyon/debug.hpp>
#include <halcyon/events.hpp>

using namespace hal;

bool event::poll()
{
    return ::SDL_PollEvent(&data) == 1;
}

void event::push()
{
    HAL_ASSERT_VITAL(::SDL_PushEvent(&data) >= 0, debug::last_error());
}

bool event::pending()
{
    return ::SDL_PollEvent(nullptr) == 1;
}

std::span<const std::uint8_t> event::keyboard_state()
{
    int size;

    const std::uint8_t* ptr { ::SDL_GetKeyboardState(&size) };
    return { ptr, static_cast<std::size_t>(size) };
}

bool event::state(type t)
{
    return ::SDL_EventState(t, SDL_QUERY);
}

bool event::state(type t, bool enable)
{
    return ::SDL_EventState(t, enable ? SDL_ENABLE : SDL_IGNORE) == SDL_ENABLE;
}