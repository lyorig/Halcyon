#include <halcyon/window.hpp>

#include <halcyon/internal/helpers.hpp>
#include <halcyon/internal/system.hpp>

using namespace hal;

window::window(std::string_view title, const pixel_point& size, std::initializer_list<flags> flags)
    : object { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, detail::il2bm<Uint32>(flags)) }
{
    HAL_PRINT(debug::init, "Initialized window \"", this->title(), "\", size ", this->size(), " at display ", hal::to_printable_int(display_index()));
}

window::window(std::string_view title, fullscreen_tag)
    : window { title, display { 0 }.size(), { fullscreen_mode } }
{
}

pixel_point window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(this->ptr(), &size.x, &size.y);

    return size;
}

void window::size(pixel_point sz)
{
    HAL_WARN_IF(fullscreen(), "Setting size of fullscreen window - this does nothing");

    ::SDL_SetWindowSize(this->ptr(), sz.x, sz.y);
}

display::index_t window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_ASSERT(ret >= 0, debug::last_error());

    return static_cast<display::index_t>(ret);
}

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

void window::title(std::string_view val)
{
    ::SDL_SetWindowTitle(ptr(), val.data());
}

bool window::fullscreen() const
{
    return static_cast<bool>(::SDL_GetWindowFlags(ptr()) & (fullscreen_mode | fullscreen_borderless));
}