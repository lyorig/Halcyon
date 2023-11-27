#ifndef NDEBUG

    #include "console.hpp"

    #include "components/font.hpp"
    #include "texture.hpp"
    #include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};

void console::draw(const font &fnt, const window &wnd) noexcept
{
    constexpr pixel_pos offset { 20, 10 };
    constexpr lyo::f64 view_height_percentage { 3.5 };

    texture tex { wnd };
    pixel_type current_height { 0 };

    for (const auto &entry : m_queue)
    {
        tex = fnt.render(entry.first, static_cast<color::hex_type>(entry.second));

        const pixel_size view { tex.vh(view_height_percentage) };

        tex.draw({ offset.x, position_type(offset.y + current_height), position_type(view.x), position_type(view.y) });

        current_height += view.y;
    }
}

#endif