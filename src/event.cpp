#include <halcyon/event.hpp>

using sub = hal::detail::subsystem<hal::detail::system::events>;

hal::mouse::state sub::mouse_proxy::state() const
{
    return { {} };
}

hal::pixel_point sub::mouse_proxy::pos_abs() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

hal::pixel_point sub::mouse_proxy::pos_rel() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

hal::keyboard::state_reference sub::keyboard_proxy::state_ref() const
{
    return { {} };
}