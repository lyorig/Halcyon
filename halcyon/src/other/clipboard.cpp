#include <SDL_clipboard.h>
#include <halcyon/debug.hpp>
#include <halcyon/other/clipboard.hpp>

std::string hal::clipboard()
{
    const char* ret { ::SDL_GetClipboardText() };
    HAL_WARN_IF(lyo::is_c_string_empty(ret), "Clipboard string empty; ", debug::last_error());

    return ret;
}

void hal::clipboard(std::string_view text)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(text.data()) == 0, debug::last_error());
}