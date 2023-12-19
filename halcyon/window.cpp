#include "window.hpp"

#include <halcyon/internal/helpers.hpp>

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, il<flags> w_flags, il<renderer::flags> r_flags)
    : sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, il2bm<Uint32>(w_flags)) }
    , renderer { *this, il2bm<lyo::u32>(r_flags), {} }
{
    HAL_DEBUG_PRINT(debug::init, "Initialized window '", title, "' (", this->size().x, 'x', this->size().y, ')');
}

window::window(engine& engine, const char* title, fullscreen_mode_tag,
    il<renderer::flags> r_flags)
    : window { engine, title, {}, {}, { fullscreen }, r_flags }
{
}

void window::present() const
{
    renderer.present({});
    renderer.clear({});
}

void window::set_as_target() const
{
    renderer.reset_target();
}

pixel_size window::size() const
{
    return renderer.output_size();
}

window::id_type window::id() const
{
    const id_type id { ::SDL_GetWindowID(m_object.get()) };

    HAL_DEBUG_ASSERT(id != 0, ::SDL_GetError());

    return id;
}

const char* window::title() const
{
    return ::SDL_GetWindowTitle(this->ptr());
}

pixel_size window::internal_size() const
{
    point<int> sz;

    ::SDL_GetWindowSize(m_object.get(), &sz.x, &sz.y);

    return static_cast<pixel_size>(sz);
}
