#include <halcyon/window.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(video& sys, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> flags)
    : sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, il2bm<Uint32>(flags)) }
{
    HAL_DEBUG_PRINT(debug::init, "Initialized window \"", this->title(), "\", size ", this->size(), " at ", sys.display_at(this->display_index()));
}

window::window(video& sys, const char* title, fullscreen_mode_tag)
    : window { sys, title, {}, sys.display_at(0).size(), { fullscreen } }
{
}
pixel_size window::size() const
{
    point<int> size;

    ::SDL_GetWindowSize(this->ptr(), &size.x, &size.y);

    return size;
}

display::index window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_DEBUG_ASSERT(ret >= 0, ::SDL_GetError());

    return ret;
}

const char* window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}