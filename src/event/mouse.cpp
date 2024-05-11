#include <halcyon/event/mouse.hpp>

#include <utility>

#include <halcyon/debug.hpp>

using namespace hal;

mouse::state::state(pass_key<authority_t>)
    : m_state { static_cast<u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(std::uint32_t mask, pass_key<event::mouse_motion_event>)
    : m_state { static_cast<u8>(mask) }
{
}

bool mouse::state::operator[](button btn) const
{
    return m_state & SDL_BUTTON(std::to_underlying(btn));
}