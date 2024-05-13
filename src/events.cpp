#include <halcyon/events.hpp>

using namespace hal;

proxy::mouse::mouse(pass_key<authority_t>)
{
}

mouse::state proxy::mouse::state() const
{
    return { pass_key<proxy::mouse> {} };
}

pixel_point proxy::mouse::pos_abs() const
{
    sdl::pixel_point ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y));

    return ret;
}

pixel_point proxy::mouse::pos_rel() const
{
    sdl::pixel_point ret;

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

void proxy::events::text_input_start()
{
    ::SDL_StartTextInput();
}

void proxy::events::text_input_stop()
{
    ::SDL_StopTextInput();
}

proxy::events::subsystem()
    : mouse { pass_key<proxy::events> {} }
    , keyboard { pass_key<proxy::events> {} }
{
}