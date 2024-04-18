#include <halcyon/debug.hpp>
#include <halcyon/event/handler.hpp>

using namespace hal;

event::handler::handler(authority&)
    : m_event { std::numeric_limits<std::uint32_t>::max() } // Start with an invalid event.
{
    // Disable unused events.
    for (SDL_EventType type : {
             SDL_LOCALECHANGED,
             SDL_SYSWMEVENT,
             SDL_TEXTEDITING,
             SDL_TEXTINPUT,
             SDL_KEYMAPCHANGED,
             SDL_TEXTEDITING_EXT,
             SDL_JOYAXISMOTION,
             SDL_JOYBALLMOTION,
             SDL_JOYHATMOTION,
             SDL_JOYBUTTONDOWN,
             SDL_JOYBUTTONUP,
             SDL_JOYDEVICEADDED,
             SDL_JOYDEVICEREMOVED,
             SDL_JOYBATTERYUPDATED,
             SDL_CONTROLLERAXISMOTION,
             SDL_CONTROLLERBUTTONDOWN,
             SDL_CONTROLLERBUTTONUP,
             SDL_CONTROLLERDEVICEADDED,
             SDL_CONTROLLERDEVICEREMOVED,
             SDL_CONTROLLERDEVICEREMAPPED,
             SDL_CONTROLLERTOUCHPADDOWN,
             SDL_CONTROLLERTOUCHPADMOTION,
             SDL_CONTROLLERTOUCHPADUP,
             SDL_CONTROLLERSENSORUPDATE,
             SDL_CONTROLLERSTEAMHANDLEUPDATED,
             SDL_FINGERDOWN,
             SDL_FINGERUP,
             SDL_FINGERMOTION,
             SDL_DOLLARGESTURE,
             SDL_DOLLARRECORD,
             SDL_MULTIGESTURE,
             SDL_DROPFILE,
             SDL_DROPTEXT,
             SDL_DROPBEGIN,
             SDL_DROPCOMPLETE,
             SDL_AUDIODEVICEADDED,
             SDL_AUDIODEVICEREMOVED,
             SDL_SENSORUPDATE,
             SDL_RENDER_TARGETS_RESET,
             SDL_RENDER_DEVICE_RESET })
    {
        ::SDL_EventState(type, SDL_IGNORE);
    }
}

bool event::handler::poll()
{
    return ::SDL_PollEvent(reinterpret_cast<SDL_Event*>(&m_event.data)) == 1;
}

event::type event::handler::event_type() const
{
    return static_cast<event::type>(m_event.data.type);
}

bool event::handler::pending()
{
    return ::SDL_PollEvent(nullptr) == 1;
}

const event::display& event::handler::display() const
{
    HAL_ASSERT(event_type() == event::type::display_event, "Invalid type");
    return m_event.data.display;
}

const event::window& event::handler::window() const
{
    HAL_ASSERT(event_type() == event::type::window_event, "Invalid type");
    return m_event.data.window;
}

const event::keyboard& event::handler::keyboard() const
{
    HAL_ASSERT(event_type() == event::type::key_pressed || event_type() == event::type::key_released, "Invalid type");
    return m_event.data.key;
}

const event::mouse_motion& event::handler::mouse_motion() const
{
    HAL_ASSERT(event_type() == event::type::mouse_moved, "Invalid type");
    return m_event.data.motion;
}

const event::mouse_button& event::handler::mouse_button() const
{
    HAL_ASSERT(event_type() == event::type::mouse_pressed || event_type() == event::type::mouse_released, "Invalid type");
    return m_event.data.button;
}

const event::mouse_wheel& event::handler::mouse_wheel() const
{
    HAL_ASSERT(event_type() == event::type::mouse_wheel_moved, "Invalid type");
    return m_event.data.wheel;
}

std::string_view hal::to_string(event::type evt)
{
    using enum event::type;

    switch (evt)
    {
    case quit_requested:
        return "Quit requested";

    case terminated:
        return "Terminated";

    case low_memory:
        return "Low memory";

    case will_enter_background:
        return "Will enter background";

    case entered_background:
        return "Entered background";

    case will_enter_foreground:
        return "Will enter foreground";

    case entered_foreground:
        return "Will enter foreground";

    case display_event:
        return "Display";

    case window_event:
        return "Window";

    case key_pressed:
        return "Key pressed";

    case key_released:
        return "Key released";

    case mouse_moved:
        return "Mouse moved";

    case mouse_pressed:
        return "Mouse pressed";

    case mouse_released:
        return "Mouse released";

    case mouse_wheel_moved:
        return "Mouse wheel moved";

    case clipboard_updated:
        return "Clipboard updated";
    }
}
