#include "window.hpp"

using namespace hal;

window::window(engine& engine, const char* title, const pixel_size& size, lyo::u32 window_flags, lyo::u32 renderer_flags) noexcept :
    sdl_object { ::SDL_CreateWindow(title, 0, 0, size.x, size.y, window_flags) },
    renderer { *this, renderer_flags },
    m_size { renderer.output_size() }
{
    HAL_DEBUG_PRINT(debug::info, "Creating window '", title, "' (", m_size.x, 'x', m_size.y, ')');
}

void window::present() noexcept
{
    m_delta.reset();

    renderer.present();
    renderer.clear();
}

const pixel_size& window::size() const noexcept
{
    return m_size;
}

double window::delta_time() const noexcept
{
    return m_delta();
}