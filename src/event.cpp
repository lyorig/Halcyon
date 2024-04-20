#include <halcyon/event.hpp>

using namespace hal::event;

using sub = hal::detail::subsystem<hal::detail::system::events>;

hal::mouse::state sub::mouse_proxy::state() const
{
    return { {} };
}

hal::coord_point sub::mouse_proxy::pos_abs() const
{
    hal::point<float> ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

hal::coord_point sub::mouse_proxy::pos_rel() const
{
    hal::point<float> ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

hal::keyboard::state_reference sub::keyboard_proxy::state_ref() const
{
    return { {} };
}