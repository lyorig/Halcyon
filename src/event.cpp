#include <halcyon/event.hpp>

using namespace hal;

using sub = detail::subsystem<detail::system::events>;

mouse::state sub::mouse_proxy::state() const
{
    return { {} };
}

pixel_point sub::mouse_proxy::pos_abs() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel_point sub::mouse_proxy::pos_rel() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

keyboard::state_reference sub::keyboard_proxy::state_ref() const
{
    return { {} };
}