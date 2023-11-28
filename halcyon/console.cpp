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
    constexpr coordinate offset { 20.0, 10.0 };
    constexpr lyo::f64 view_height_percentage { 3.5 };

    texture tex { wnd };

    // Could be static, but you never know if someone has the bright idea
    // of rendering the console with multiple fonts.
    const lyo::f64 scale { (wnd.size().y * (view_height_percentage / 100.0)) / fnt.size_text("").y };

    for (count_type i { 0 }; i < m_entries; ++i) {
        const value_pair& entry { m_queue[i] };

        tex = fnt.render(entry.first, static_cast<color::hex_type>(entry.second))
                  .resize(scale);
        tex.draw({ offset.x, offset.y + tex.size().y * i });
    }
}

#endif