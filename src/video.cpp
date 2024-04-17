#include <halcyon/video.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal::video;

system::system(context& auth)
    : subinit { auth }
    , events { auth }
{
}

std::string system::clipboard_proxy::operator()() const
{
    const auto text = ::SDL_GetClipboardText();

    HAL_ASSERT(text[0] != '\0' || has_text(), debug::last_error());

    return text;
};

void system::clipboard_proxy::operator()(std::string_view text)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(text.data()) == 0, debug::last_error());
}

bool system::clipboard_proxy::has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

display::id_t system::display_proxy::size() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret >= 1, debug::last_error());

    return static_cast<display::id_t>(ret);
}

display system::display_proxy::operator[](display::id_t idx) const
{
    return { idx, {} };
}