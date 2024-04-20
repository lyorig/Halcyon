#include <halcyon/event/mouse.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

mouse::state::state()
    : m_state { static_cast<u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(std::uint32_t mask, pass_key<event::mouse_motion>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask, pass_key<event::mouse_button>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask)
    : m_state { static_cast<u8>(mask) }
{
}

bool mouse::state::operator[](button btn) const
{
    return m_state & SDL_BUTTON(std::to_underlying(btn));
}

coord_point mouse::pos_rel()
{
    hal::point<float> ret;

    static_cast<void>(::SDL_GetMouseState(&ret.x, &ret.y));

    return ret;
}

coord_point mouse::pos_abs()
{
    hal::point<float> ret;

    static_cast<void>(::SDL_GetGlobalMouseState(&ret.x, &ret.y) != std::numeric_limits<std::uint32_t>::max());

    return ret;
}