#ifndef NDEBUG
    #include "debug.hpp"

    #include <SDL2/SDL_messagebox.h>

    #include <cstring>

using namespace halcyon;

/* Static private variables. */
std::ofstream            debug::m_output { "Halcyon debug output.txt" };
const lyo::precise_timer debug::m_timer {};

void debug::panic(const char* reason) noexcept
{
    const char* err { std::strlen(::SDL_GetError()) ? ::SDL_GetError() : "no SDL error" };
    debug::print(severity::error, __func__, ": ", reason, " - ", err);

    const SDL_MessageBoxButtonData buttons[] {
        {SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT,  0, "Exit"      },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Run anyway"}
    };

    const SDL_MessageBoxData msgbox { SDL_MESSAGEBOX_ERROR, NULL, reason, err, SDL_arraysize(buttons), buttons, NULL };

    int response { 0 };

    if (::SDL_ShowMessageBox(&msgbox, &response) < 0)
        debug::print(severity::error, __func__, ": Message box creation failed, exiting");

    else
        debug::print(severity::info, __func__, ": User chose to ", response == 0 ? "exit" : "continue execution");

    if (response == 0)
        std::exit(EXIT_FAILURE);
}

void debug::verify(bool condition, const char* if_false) noexcept
{
    if (!condition)
        debug::panic(if_false);
}
#endif