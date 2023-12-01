#include "halcyon/debug.hpp"
#ifndef NDEBUG

#include "console.hpp"

#include "components/font.hpp"
#include "texture.hpp"
#include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};
console::count_type console::m_entries { 0 };

void console::draw(const font& fnt, const window& wnd)
{
    // Render settings.
    constexpr coordinate offset { 20.0, 10.0 };
    constexpr lyo::f64 vhm { 3.5 / 100.0 }; // View-height multiplier (only edit the lhs).

    pixel_type y_size { fnt.size_text("").y };
    pixel_size csz { .y = pixel_type(y_size * m_entries) }; // Canvas size.

    // "Calculate" canvas size.
    for (count_type i { 0 }; i < m_entries; ++i) {
        const pixel_type x_size { fnt.size_text(m_queue[i].first).x };
        if (x_size > csz.x)
            csz.x = x_size;
    }

    // Compose the texture.
    surface canvas { wnd, csz };

    for (count_type i { 0 }; i < m_entries; ++i) {
        const value_pair& entry { m_queue[i] };
        const surface text { fnt.render(entry.first, static_cast<color::hex_type>(entry.second)) };

        hal::surface::drawer(text).to({ 0, pixel_type(text.size().y * i) })(canvas);
    }

    hal::texture::drawer({ wnd, canvas }).to(offset).scale((wnd.size().y * vhm) / y_size)();
}

#endif
