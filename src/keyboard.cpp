#include <halcyon/keyboard.hpp>
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

std::string_view hal::to_string(keyboard::button btn)
{
    return ::SDL_GetScancodeName(static_cast<SDL_Scancode>(btn));
}

std::string_view hal::to_string(keyboard::key k)
{
    return ::SDL_GetKeyName(static_cast<SDL_KeyCode>(k));
}