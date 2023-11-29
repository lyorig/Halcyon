#include "halcyon/debug.hpp"
#ifndef NDEBUG

#include "console.hpp"

#include "components/font.hpp"
#include "texture.hpp"
#include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};
console::count_type console::m_entries { 0 };

void console::draw(const font& fnt, const window& wnd) noexcept
{
    constexpr coordinate offset { 10.0, 10.0 };
    constexpr lyo::f64 view_height_percentage { 3.5 };

    pixel_size canvas_size { .y = static_cast<pixel_type>(fnt.size_text("").y * m_entries) };

    // Could be static, but you never know if someone'll have the bright
    // idea to render the console with multiple fonts.
    const lyo::f64 scale { (wnd.size().y * (view_height_percentage * m_queue.size() / 100.0)) / canvas_size.y };

    // Find out surface dimensions.
    for (count_type i { 0 }; i < m_entries; ++i) {
        const pixel_type xlen { fnt.size_text(m_queue[i].first.c_str()).x };

        if (xlen > canvas_size.x)
            canvas_size.x = xlen;
    }

    std::cout << "Size is " << canvas_size.x << 'x' << canvas_size.y << '\n';

    // Draw everything onto a canvas.
    surface canvas { wnd, canvas_size * scale };

    for (count_type i { 0 }; i < m_entries; ++i) {
        const value_pair& entry { m_queue[i] };

        const surface surf { fnt.render(entry.first, static_cast<color::hex_type>(entry.second)).resize(scale) };
        const pixel_size sz { surf.size() };

        SDL_Rect dest { 0, sz.y * i, sz.x, sz.y };

        surf.set_blend(SDL_BLENDMODE_NONE);

        HAL_DEBUG_ASSERT(::SDL_BlitSurface(surf.ptr(), nullptr, canvas.ptr(), &dest) == 0, ::SDL_GetError());
    }

    hal::texture { wnd, canvas }.draw(offset);
}

#endif
