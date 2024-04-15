#include <halcyon/video.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

video::video(context& auth)
    : subinit { auth }
    , events { auth }
{
}

std::string video::clipboard_proxy::operator()() const
{
    const auto text = ::SDL_GetClipboardText();

    HAL_ASSERT(!lyo::is_c_string_empty(text) || has_text(), debug::last_error());

    return text;
};

void video::clipboard_proxy::operator()(std::string_view text)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(text.data()) == 0, debug::last_error());
}

bool video::clipboard_proxy::has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

display::index_t video::display_proxy::size() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret >= 1, debug::last_error());

    return static_cast<display::index_t>(ret);
}

display video::display_proxy::operator[](display::index_t idx) const
{
    return { idx, {} };
}