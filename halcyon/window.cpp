#include "window.hpp"

#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags)
    : sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, il2bm<Uint32>(w_flags)) }
    , renderer { *this, il2bm<Uint32>(r_flags), {} }
{
    HAL_DEBUG_PRINT(debug::init, "Initialized window ", title, ", size ", this->size());
}

// This is a bit of a bodge. I do not like the split call to display(),
// and it'll hopefully eventually be replaced with something better.
// The problem is that if the constructor is delegated, the subsystem isn't
// initialized by the time display() is called, which causes errors.
window::window(engine& engine, const char* title, fullscreen_mode_tag, il<renderer::flags> r_flags)
    : sdl_object { ::SDL_CreateWindow(title, 0, 0, this->display(0).size.x, this->display(0).size.y, fullscreen) }
    , renderer { *this, il2bm<Uint32>(r_flags), {} }
{
}

void window::present()
{
    renderer.present({});
    renderer.clear({});
}

void window::set_size(pixel_size sz)
{
    // Returns void, no error checking required.
    ::SDL_SetWindowSize(this->ptr(), sz.x, sz.y);
}

void window::set_as_target()
{
    renderer.reset_target();
}

pixel_size window::size() const
{
    return renderer.output_size();
}

display_info::index window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_DEBUG_ASSERT(ret >= 0, ::SDL_GetError());

    return display_info::index(ret);
}

const char* window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

pixel_size window::internal_size() const
{
    point<int> sz;

    // Returns void, no error checking required.
    ::SDL_GetWindowSize(this->ptr(), &sz.x, &sz.y);

    return static_cast<pixel_size>(sz);
}