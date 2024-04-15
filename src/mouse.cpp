#include <halcyon/mouse.hpp>
#include <utility>

using namespace hal;

mouse::state::state()
    : m_state { static_cast<lyo::u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<event::mouse_motion>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<event::mouse_button>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask)
    : m_state { static_cast<lyo::u8>(mask) }
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

std::string_view hal::to_string(mouse::button btn)
{
    using enum hal::mouse::button;

    switch (btn)
    {
    case left:
        return "Left Mouse";

    case right:
        return "Right Mouse";

    case middle:
        return "Middle Mouse";

    case x1:
        return "Mouse Extra 1";

    case x2:
        return "Mouse Extra 2";
    }
}