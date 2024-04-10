#include <SDL_clipboard.h>
#include <halcyon/debug.hpp>
#include <halcyon/other/clipboard.hpp>

std::string hal::clipboard::get()
{
    const char* ret { ::SDL_GetClipboardText() };
    HAL_ASSERT(has_text() || !lyo::is_c_string_empty(ret), debug::last_error());

    return ret;
}

void hal::clipboard::set(std::string_view text)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(text.data()) == 0, debug::last_error());
}

bool hal::clipboard::has_text()
{
    return ::SDL_HasClipboardText() == SDL_TRUE ? true : false;
}