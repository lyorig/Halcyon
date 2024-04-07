#include <halcyon/types/events.hpp>

using namespace hal;

// Display event.

events::display::event_type events::display::type() const
{
    return static_cast<event_type>(event);
}

display::index_t events::display::display_index() const
{
    return static_cast<hal::display::index_t>(SDL_DisplayEvent::display);
}

events::display::payload_t events::display::data() const
{
    return static_cast<payload_t>(data1);
}

// Window event.

window::id_t events::window::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

events::window::event_type events::window::type() const
{
    return static_cast<event_type>(event);
}

// Keyboard event.

window::id_t events::keyboard::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

keyboard::button events::keyboard::button() const
{
    return static_cast<hal::keyboard::button>(keysym.scancode);
}

bool events::keyboard::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

// Mouse motion event.

window::id_t events::mouse_motion::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

mouse::state events::mouse_motion::state() const
{
    return SDL_MouseMotionEvent::state;
}

pixel_point events::mouse_motion::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

pixel_point events::mouse_motion::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

// Mouse button event.

window::id_t events::mouse_button::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

mouse::state events::mouse_button::state() const
{
    return SDL_MouseButtonEvent::state;
}

mouse::button events::mouse_button::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

lyo::u8 events::mouse_button::click_amount() const
{
    return static_cast<lyo::u8>(clicks);
}

pixel_point events::mouse_button::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

// Mouse wheel event.

window::id_t events::mouse_wheel::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

point<lyo::f32> events::mouse_wheel::scroll() const
{
    return { static_cast<lyo::f32>(preciseX), static_cast<lyo::f32>(preciseY) };
}

pixel_point events::mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

bool events::mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}