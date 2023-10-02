#include "window.hpp"
#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags) noexcept
    : sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, il2bm<Uint32>(w_flags)) }
    , renderer { *this, il2bm<lyo::u32>(r_flags), {} }
{
    HAL_DEBUG_PRINT(severity::init, "Initialized window '", title, "' (", this->size().x, 'x', this->size().y, ')');
}

window::window(engine &engine, const char *title, fullscreen_tag,
               il<renderer::flags> r_flags) noexcept
    : window{engine, title, {}, {}, {fullscreen}, r_flags} {
}

void window::present() noexcept
{
    renderer.present({});
    renderer.clear({});
}

void window::set_as_target() noexcept
{
    renderer.reset_target();
}

pixel_size window::size() const noexcept
{
    return renderer.output_size();
}