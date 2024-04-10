#include <halcyon/other/keyboard_mouse.hpp>
#include <utility>

using namespace hal;

keyboard::key keyboard::to_key(button btn)
{
    return static_cast<keyboard::key>(::SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(btn)));
}

keyboard::button keyboard::to_button(key k)
{
    return static_cast<keyboard::button>(::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k)));
}

keyboard::state_reference::state_reference()
    : m_arr { ::SDL_GetKeyboardState(nullptr) }
{
}

bool keyboard::state_reference::operator[](button btn) const
{
    return m_arr[std::to_underlying(btn)];
}

bool keyboard::state_reference::operator[](key k) const
{
    return m_arr[::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k))];
}

mouse::state::state()
    : m_state { static_cast<lyo::u8>(::SDL_GetMouseState(nullptr, nullptr)) }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<events::mouse_motion>)
    : state { mask }
{
}

mouse::state::state(std::uint32_t mask, lyo::pass_key<events::mouse_button>)
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

std::string_view hal::to_string(keyboard::button btn)
{
    return ::SDL_GetScancodeName(static_cast<SDL_Scancode>(btn));
}

std::string_view hal::to_string(keyboard::key k)
{
    return ::SDL_GetKeyName(static_cast<SDL_KeyCode>(k));
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