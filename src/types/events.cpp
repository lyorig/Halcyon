#include <halcyon/types/events.hpp>

using namespace hal;

// Display event.

display_event events::display::type() const
{
    return static_cast<display_event>(event);
}

display::index_t events::display::display_index() const
{
    return static_cast<hal::display::index_t>(SDL_DisplayEvent::display);
}

// Window event.

window::id_t events::window::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

window_event events::window::type() const
{
    return static_cast<window_event>(event);
}

hal::display::index_t events::window::new_display_index() const
{
    HAL_ASSERT(type() == window_event::display_changed, "Invalid event type");
    return static_cast<hal::display::index_t>(data1);
}

pixel_point events::window::new_point() const
{
    HAL_ASSERT(type() == window_event::resized || type() == window_event::moved, "Invalid event type");
    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
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

keyboard::key events::keyboard::key() const
{
    return static_cast<hal::keyboard::key>(keysym.sym);
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
    return { SDL_MouseMotionEvent::state, lyo::pass_key<mouse_motion> {} };
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
    return { SDL_MouseButtonEvent::state, lyo::pass_key<mouse_button> {} };
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