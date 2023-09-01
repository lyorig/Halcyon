#include "window.hpp"

#include "input_handler.hpp"

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& pos, const pixel_size& size, lyo::u32 window_flags, lyo::u32 renderer_flags) noexcept :
    sdl_object { ::SDL_CreateWindow(title, pos.x, pos.y, size.x, size.y, window_flags) },
    renderer { *this, renderer_flags, {} },
    m_flags { window_flags }
{
    HAL_DEBUG_PRINT(severity::init, "Initialized window '", title, "' (", this->size().x, 'x', this->size().y, ')');
}

window::window(engine& engine, const char* title, fullscreen_t, lyo::u32 renderer_flags) noexcept :
    window { engine, title, {}, {}, fullscreen, renderer_flags }
{
}

void window::present() noexcept
{
    renderer.present({});
    renderer.clear({});
}

pixel_size window::size() const noexcept
{
    return renderer.output_size();
}