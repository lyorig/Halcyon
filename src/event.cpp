#include <halcyon/event.hpp>

using namespace hal;

using sub = detail::subsystem<detail::system::events>;

detail::mouse_proxy::mouse_proxy(pass_key<authority_t>)
{
}

mouse::state detail::mouse_proxy::state() const
{
    return { pass_key<mouse_proxy> {} };
}

pixel_point detail::mouse_proxy::pos_abs() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel_point detail::mouse_proxy::pos_rel() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

detail::keyboard_proxy::keyboard_proxy(pass_key<authority_t>)
{
}

keyboard::state_reference detail::keyboard_proxy::state_ref() const
{
    return pass_key<keyboard_proxy> {};
}

sub::subsystem(pass_key<authority_t>)
    : subsystem {}
{
}

sub::subsystem(pass_key<parent_t>)
    : subsystem {}
{
}

sub::subsystem()
    : mouse { pass_key<sub> {} }
    , keyboard { pass_key<sub> {} }
{
}