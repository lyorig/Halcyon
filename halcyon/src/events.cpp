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

std::span<const Uint8> event::keyboard_state()
{
    int size;

    const auto ptr = ::SDL_GetKeyboardState(&size);

    return { ptr, static_cast<std::size_t>(size) };
}

bool event::state(type t)
{
    return ::SDL_EventState(t, SDL_QUERY);
}

void event::state(type t, bool enable)
{
    ::SDL_EventState(t, enable ? SDL_ENABLE : SDL_IGNORE);
}