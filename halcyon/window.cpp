#include "window.hpp"

#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags)
    : sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, il2bm<Uint32>(w_flags)) }
    , renderer { *this, il2bm<Uint32>(r_flags), {} }
{
    HAL_DEBUG_PRINT(debug::init, "Initialized window ", title, ", size ", this->size());
}

// Okay, simply put: Proper fullscreen is a pain in the ass, and I
// truly regret ever trying to implement it.
window::window(engine& engine, const char* title, fullscreen_mode_tag, il<renderer::flags> r_flags)
    : window { engine, title, {}, {}, { fullscreen }, r_flags }
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

window::id_type window::id() const
{
    const auto ret { ::SDL_GetWindowID(this->ptr()) };

    HAL_DEBUG_ASSERT(ret != 0, ::SDL_GetError());

    return window::id_type(ret);
}

display::index_type window::display_index() const
{
    const auto ret = ::SDL_GetWindowDisplayIndex(this->ptr());

    HAL_DEBUG_ASSERT(ret >= 0, ::SDL_GetError());

    return display::index_type(ret);
}

bool window::is_fullscreen() const
{
    return ::SDL_GetWindowFlags(this->ptr()) & fullscreen;
}

const char* window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

void window::set_display_mode(const SDL_DisplayMode& mode)
{
    HAL_DEBUG_ASSERT(this->is_fullscreen(), "Changing display mode of non-fullscreen window");
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetWindowDisplayMode(this->ptr(), &mode) == 0, ::SDL_GetError());
}

pixel_size window::internal_size() const
{
    point<int> sz;

    // Returns void, no error checking required.
    ::SDL_GetWindowSize(this->ptr(), &sz.x, &sz.y);

    return static_cast<pixel_size>(sz);
}