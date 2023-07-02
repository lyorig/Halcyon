#ifndef NDEBUG

    #include "debug.hpp"

    #include <SDL2/SDL_messagebox.h>

    #include <cstring>
    #include <lyo/utility.hpp>

using namespace hal;

// Static private variables.
std::ofstream            debug::m_output { "Halcyon debug output.txt" };
const lyo::precise_timer debug::m_timer {};

void debug::panic(const char* title, const char* message) noexcept
{
    const char* fmt_msg { lyo::is_c_string_empty(message) ? "No message provided" : message };

    debug::print(severity::error, __func__, ": ", title, " <- ", fmt_msg);

    constexpr SDL_MessageBoxButtonData buttons[] {
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,  0, "Exit"      },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Run anyway"}
    };

    const SDL_MessageBoxData msgbox { SDL_MESSAGEBOX_ERROR, NULL, title, fmt_msg, SDL_arraysize(buttons), buttons, NULL };

    int response { 0 };

    if (::SDL_ShowMessageBox(&msgbox, &response) < 0)
    {
        debug::print(severity::error, __func__, ": Message box creation failed, exiting");
        goto Exit;
    }

    else
        debug::print(severity::info, __func__, ": User chose to ", response == 0 ? "exit" : "continue execution");

    if (response == 0)
    {
    Exit:
        std::exit(EXIT_FAILURE);
    }
}

void debug::verify(bool condition, const char* if_false, const char* info) noexcept
{
    if (!condition)
        debug::panic(if_false, info);
}
#endif