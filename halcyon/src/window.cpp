#include <halcyon/system.hpp>
#include <halcyon/window.hpp>

#include <halcyon/other/helpers.hpp>

using namespace hal;

window::window(std::string_view title, const pixel_point& size, std::initializer_list<flags> flags)
    : object { ::SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, il2bm<Uint32>(flags)) }
{
    HAL_PRINT(debug::init, "Initialized window \"", this->title(), "\", size ", this->size(), " at ", display_info());
}

window::window(std::string_view title, fullscreen_mode_tag)
    : window { title, display { 0 }.size(), { fullscreen } }
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
    ::SDL_SetWindowSize(this->ptr(), sz.x, sz.y);
}

display window::display_info() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_ASSERT(ret >= 0, ::SDL_GetError());

    return display { static_cast<display::index_t>(ret) };
}

std::string_view window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}