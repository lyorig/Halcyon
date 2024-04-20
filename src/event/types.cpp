#include <halcyon/event/types.hpp>

using namespace hal::event;

namespace
{
    using window_id  = hal::video::window::id_t;
    using display_id = hal::video::display::id_t;
}

// Display event.

enum display::type display::event_type() const
{
    return static_cast<enum type>(event);
}

display_id display::display_index() const
{
    return static_cast<display_id>(SDL_DisplayEvent::display);
}

// Window event.

window_id window::window_id() const
{
    return static_cast<::window_id>(windowID);
}

enum window::type window::event_type() const
{
    return static_cast<enum type>(event);
}

display_id window::new_display_index() const
{
    HAL_ASSERT(event_type() == type::display_changed, "Invalid event type");
    return static_cast<display_id>(data1);
}

hal::pixel_point window::new_point() const
{
    HAL_ASSERT(event_type() == type::resized || event_type() == type::moved, "Invalid event type");
    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
}

// Keyboard event.

window_id keyboard::window_id() const
{
    return static_cast<::window_id>(windowID);
}

hal::keyboard::button keyboard::button() const
{
    return static_cast<hal::keyboard::button>(keysym.scancode);
}

hal::keyboard::key keyboard::key() const
{
    return static_cast<hal::keyboard::key>(keysym.sym);
}

bool keyboard::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

// Mouse motion event.

window_id mouse_motion::window_id() const
{
    return static_cast<::window_id>(windowID);
}

hal::mouse::state mouse_motion::state() const
{
    return { static_cast<std::uint8_t>(SDL_MouseMotionEvent::state), pass_key<mouse_motion> {} };
}

hal::pixel_point mouse_motion::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

hal::pixel_point mouse_motion::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

// Mouse button event.

window_id mouse_button::window_id() const
{
    return static_cast<::window_id>(windowID);
}

hal::mouse::button mouse_button::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

hal::u8 mouse_button::click_amount() const
{
    return static_cast<u8>(clicks);
}

hal::pixel_point mouse_button::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

// Mouse wheel event.

window_id mouse_wheel::window_id() const
{
    return static_cast<::window_id>(windowID);
}

hal::point<hal::i16> mouse_wheel::scroll() const
{
    return { static_cast<i16>(x), static_cast<i16>(y) };
}

hal::point<hal::f32> mouse_wheel::scroll_precise() const
{
    return { static_cast<f32>(preciseX), static_cast<f32>(preciseY) };
}

hal::pixel_point mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

bool mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}

std::string_view hal::to_string(enum display::type evt)
{
    using enum display::type;

    switch (evt)
    {
    case reoriented:
        return "Reoriented";

    case connected:
        return "Connected";

    case disconnected:
        return "Disconnected";

    case moved:
        return "Moved";
    }
}

std::string_view hal::to_string(enum window::type evt)
{
    using enum window::type;

    switch (evt)
    {
    case shown:
        return "Shown";

    case hidden:
        return "Hidden";

    case exposed:
        return "Exposed";

    case moved:
        return "Moved";

    case resized:
        return "Resized";

    case minimized:
        return "Minimized";

    case maximized:
        return "Maximized";

    case restored:
        return "Restored";

    case got_mouse_focus:
        return "Got mouse focus";

    case lost_mouse_focus:
        return "Lost mouse focus";

    case got_keyboard_focus:
        return "Got keyboard focus";

    case lost_keyboard_focus:
        return "Lost keyboard focus";

    case closed:
        return "Closed";

    case focus_offered:
        return "Focus offered";

    case hit_test:
        return "Hit test";

    case icc_profile_changed:
        return "ICC profile changed";

    case display_changed:
        return "Display changed";
    }
}