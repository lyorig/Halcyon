#include "window.hpp"

#include "input_handler.hpp"

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& size, lyo::u32 window_flags, lyo::u32 renderer_flags) noexcept :
    sdl_object { ::SDL_CreateWindow(title, 0, 0, size.x, size.y, window_flags) },
    renderer { *this, renderer_flags, {} },
    m_size { renderer.output_size() }
{
    HAL_DEBUG_PRINT(success, "Initialized window '", title, "' (", m_size.x, 'x', m_size.y, ')');
}

window::window(engine& engine, const char* title, fullscreen_t, lyo::u32 renderer_flags) noexcept :
    sdl_object { ::SDL_CreateWindow(title, 0, 0, 0, 0, static_cast<Uint32>(fullscreen)) },
    renderer { *this, renderer_flags, {} },
    m_size { renderer.output_size() }
{
    HAL_DEBUG_PRINT(success, "Initialized fullscreen window '", title, "' (", m_size.x, 'x', m_size.y, ')');
}

void window::present() noexcept
{
    m_delta.reset();

    renderer.present({});
    renderer.clear({});
}

const pixel_size& window::size() const noexcept
{
    return m_size;
}

double window::delta_time() const noexcept
{
    return m_delta();
}