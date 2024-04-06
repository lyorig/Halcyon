#include <halcyon/debug.hpp>
#include <halcyon/events.hpp>

using namespace hal;

bool event::poll()
{
    if (::SDL_PollEvent(&m_event) == 0)
        return false;

    switch (static_cast<enum type>(evt.type))
    {
        case 
    }

    return true;
}

void event::push()
{
    HAL_ASSERT_VITAL(::SDL_PushEvent(&data) >= 0, debug::last_error());
}

event::system_event event::type() const
{
    return static_cast<event::system_event>(data.common.type);
}

event::tick_t event::timestamp() const
{
    return static_cast<event::tick_t>(data.common.timestamp);
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

bool event::state(system_event t)
{
    return ::SDL_EventState(t, SDL_QUERY);
}

bool event::state(system_event t, bool enable)
{
    return ::SDL_EventState(t, enable ? SDL_ENABLE : SDL_IGNORE) == SDL_ENABLE;
}