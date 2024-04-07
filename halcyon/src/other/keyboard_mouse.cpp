#include <halcyon/other/keyboard_mouse.hpp>
#include <utility>

using namespace hal;

keyboard::state::state(const std::uint8_t* ptr)
    : m_arr { ptr }
{
}

bool keyboard::state::operator[](button btn) const
{
    return m_arr[std::to_underlying(btn)];
}

mouse::state::state(std::uint32_t state)
    : m_state { static_cast<lyo::u8>(state) }
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