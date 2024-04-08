#include <halcyon/other/keyboard_mouse.hpp>
#include <utility>

using namespace hal;

keyboard::state_reference::state_reference()
    : m_arr { ::SDL_GetKeyboardState(nullptr) }
{
}

bool keyboard::state_reference::operator[](button btn) const
{
    return m_arr[std::to_underlying(btn)];
}

mouse::state::state()
    : m_state { static_cast<lyo::u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

bool mouse::state::operator[](button btn) const
{
    return m_state & SDL_BUTTON(std::to_underlying(btn));
}

pixel_point mouse::pos()
{
    hal::point<int> ret;

    ::SDL_GetMouseState(&ret.x, &ret.y);

    return ret;
}