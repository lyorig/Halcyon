#include <halcyon/video.hpp>

#include <string>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

using sub = hal::detail::subsystem<hal::detail::system::video>;

window sub::make_window(std::string_view title, pixel_point size, std::initializer_list<window::flags> flags)
{
    return { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size.x, size.y, to_bitmask<std::uint32_t>(flags)), {} };
}

std::string sub::clipboard_proxy::operator()() const
{
    const auto text = ::SDL_GetClipboardText();

    HAL_ASSERT(text[0] != '\0' || has_text(), debug::last_error());

    return text;
};

void sub::clipboard_proxy::operator()(std::string_view text)
{
    HAL_ASSERT_VITAL(::SDL_SetClipboardText(text.data()) == 0, debug::last_error());
}

bool sub::clipboard_proxy::has_text() const
{
    return ::SDL_HasClipboardText() == SDL_TRUE;
}

display::id_t sub::display_proxy::size() const
{
    const auto ret = ::SDL_GetNumVideoDisplays();

    HAL_ASSERT(ret >= 1, debug::last_error());

    return static_cast<display::id_t>(ret);
}

display sub::display_proxy::operator[](display::id_t idx) const
{
    return { idx, {} };
}