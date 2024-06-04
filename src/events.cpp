#include <halcyon/events.hpp>

using namespace hal;

proxy::mouse::mouse(pass_key<authority_t>)
{
}

mouse::state proxy::mouse::state() const
{
    return { pass_key<proxy::mouse> {} };
}

pixel::point proxy::mouse::pos_abs() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel::point proxy::mouse::pos_rel() const
{
    pixel::point ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

proxy::keyboard::keyboard(pass_key<authority_t>)
{
}

keyboard::state_reference proxy::keyboard::state_ref() const
{
    return pass_key<proxy::keyboard> {};
}

keyboard::mod_state proxy::keyboard::mod() const
{
    return pass_key<proxy::keyboard> {};
}

proxy::events::subsystem(pass_key<authority_t>)
    : subsystem {}
{
}

proxy::events::subsystem(pass_key<parent_t>)
    : subsystem {}
{
}

proxy::events::subsystem()
    : mouse { pass_key<proxy::events> {} }
    , keyboard { pass_key<proxy::events> {} }
{
}