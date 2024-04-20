#include <halcyon/debug.hpp>
#include <halcyon/event/handler.hpp>

#include <utility>

using namespace hal;

event::handler::handler(authority&)
    : m_event { { std::numeric_limits<std::uint32_t>::max() } } // Start with an invalid event.
{
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
    HAL_ASSERT(is_display_event(), "Invalid type");
    return m_event.data.display;
}

const event::window& event::handler::window() const
{
    HAL_ASSERT(is_window_event(), "Invalid type");
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

bool event::handler::is_window_event() const
{
    const auto val = std::to_underlying(event_type());

    return val >= SDL_EVENT_WINDOW_FIRST && val <= SDL_EVENT_WINDOW_LAST;
}

bool event::handler::is_display_event() const
{
    const auto val = std::to_underlying(event_type());

    return val >= SDL_EVENT_DISPLAY_FIRST && val <= SDL_EVENT_DISPLAY_LAST;
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

    case window_shown:
        return "Window: Shown";

    case window_hidden:
        return "Window: Hidden";

    case window_exposed:
        return "Window: Exposed";

    case window_moved:
        return "Window: Moved";

    case window_resized:
        return "Window: Resized";

    case window_minimized:
        return "Window: Minimized";

    case window_maximized:
        return "Window: Maximized";

    case window_restored:
        return "Window: Restored";

    case window_focus_gained:
        return "Window: Focus gained";

    case window_focus_lost:
        return "Window: Focus lost";

    case window_close_requested:
        return "Window: Close requestd";

    case window_focus_offered:
        return "Window: Focus offered";

    case window_hit_test:
        return "Window: Hit test";

    case window_changed_icc_profile:
        return "Window: ICC profile changed";

    case window_changed_display:
        return "Window: Window display changed";

    case display_reoriented:
        return "Display: Reoriented";

    case display_added:
        return "Display: Connected";

    case display_removed:
        return "Display: Disconnected";

    case display_moved:
        return "Display: Moved";

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

    default:
        return "[unimplemented]";
    }
}
