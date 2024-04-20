#include <halcyon/event/types.hpp>

using namespace hal::event;

namespace
{
    using window_id  = hal::video::window::id_t;
    using display_id = hal::video::display::id_t;
}

// Display event.

display_id display::display_index() const
{
    return static_cast<display_id>(SDL_DisplayEvent::displayID);
}

// Window event.

window_id window::window_id() const
{
    return static_cast<::window_id>(windowID);
}

display_id window::new_display_index() const
{
    return static_cast<display_id>(data1);
}

hal::pixel_point window::new_point() const
{
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
    return { SDL_MouseMotionEvent::state, pass_key<mouse_motion> {} };
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

hal::mouse::state mouse_button::state() const
{
    return { SDL_MouseButtonEvent::state, pass_key<mouse_button> {} };
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
    return { static_cast<f32>(x), static_cast<f32>(y) };
}

hal::pixel_point mouse_wheel::pos() const
{
    return { static_cast<pixel_t>(x), static_cast<pixel_t>(y) };
}

bool mouse_wheel::scroll_flipped() const
{
    return direction == SDL_MOUSEWHEEL_FLIPPED;
}