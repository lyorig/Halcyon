#include <halcyon/types/events.hpp>

using namespace hal;

// Display event.

enum event::display::type event::display::type() const
{
    return static_cast<enum type>(event);
}

display::index_t event::display::display_index() const
{
    return static_cast<hal::display::index_t>(SDL_DisplayEvent::display);
}

// Window event.

window::id_t event::window::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

enum event::window::type event::window::type() const
{
    return static_cast<enum type>(event);
}

hal::display::index_t event::window::new_display_index() const
{
    HAL_ASSERT(type() == type::display_changed, "Invalid event type");
    return static_cast<hal::display::index_t>(data1);
}

pixel_point event::window::new_point() const
{
    HAL_ASSERT(type() == type::resized || type() == type::moved, "Invalid event type");
    return { static_cast<pixel_t>(data1), static_cast<pixel_t>(data2) };
}

// Keyboard event.

window::id_t event::keyboard::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

keyboard::button event::keyboard::button() const
{
    return static_cast<hal::keyboard::button>(keysym.scancode);
}

keyboard::key event::keyboard::key() const
{
    return static_cast<hal::keyboard::key>(keysym.sym);
}

bool event::keyboard::repeat() const
{
    return static_cast<bool>(SDL_KeyboardEvent::repeat);
}

// Mouse motion event.

window::id_t event::mouse_motion::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

mouse::state event::mouse_motion::state() const
{
    return { SDL_MouseMotionEvent::state, lyo::pass_key<mouse_motion> {} };
}

pixel_point event::mouse_motion::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

pixel_point event::mouse_motion::rel() const
{
    return { static_cast<pixel_t>(xrel), static_cast<pixel_t>(yrel) };
}

// Mouse button event.

window::id_t event::mouse_button::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

mouse::state event::mouse_button::state() const
{
    return { SDL_MouseButtonEvent::state, lyo::pass_key<mouse_button> {} };
}

mouse::button event::mouse_button::button() const
{
    return static_cast<mouse::button>(SDL_MouseButtonEvent::button);
}

lyo::u8 event::mouse_button::click_amount() const
{
    return static_cast<lyo::u8>(clicks);
}

pixel_point event::mouse_button::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

// Mouse wheel event.

window::id_t event::mouse_wheel::window_id() const
{
    return static_cast<hal::window::id_t>(windowID);
}

point<lyo::f32> event::mouse_wheel::scroll() const
{
    return { static_cast<lyo::f32>(preciseX), static_cast<lyo::f32>(preciseY) };
}

pixel_point event::mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

bool event::mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}