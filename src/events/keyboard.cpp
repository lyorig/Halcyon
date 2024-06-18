#include <halcyon/events/keyboard.hpp>

using namespace hal;

keyboard::key keyboard::to_key(button btn)
{
    return static_cast<keyboard::key>(::SDL_GetKeyFromScancode(static_cast<SDL_Scancode>(btn)));
}

keyboard::button keyboard::to_button(key k)
{
    return static_cast<keyboard::button>(::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k)));
}

keyboard::state_reference::state_reference(pass_key<authority_t>)
    : enum_bitmask { ::SDL_GetKeyboardState(nullptr) }
{
}

bool keyboard::state_reference::operator[](key k) const
{
    return enum_bitmask::operator[](static_cast<button>(::SDL_GetScancodeFromKey(static_cast<SDL_KeyCode>(k))));
}

keyboard::mod_state::mod_state(pass_key<authority_t>)
    : enum_bitmask { ::SDL_GetModState() }
{
}

std::string_view hal::to_string(keyboard::button btn)
{
    return ::SDL_GetScancodeName(static_cast<SDL_Scancode>(btn));
}

std::string_view hal::to_string(keyboard::key k)
{
    return ::SDL_GetKeyName(static_cast<SDL_KeyCode>(k));
}