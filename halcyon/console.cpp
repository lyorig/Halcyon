#ifndef NDEBUG

    #include "console.hpp"

    #include "components/font.hpp"
    #include "texture.hpp"
    #include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};

void console::draw(const font& fnt, const window& wnd) noexcept
{
    constexpr coordinate offset { 20.0, 10.0 };
    constexpr lyo::f64   view_height_percentage { 3.5 };

    texture tex { wnd };

    position_type current_height { 0.0 };

    for (const auto& entry : m_queue)
    {
        tex = fnt.render(entry.first, static_cast<color>(entry.second));

        const pixel_size view { tex.vh(view_height_percentage) };

        tex.draw({ offset.x, offset.y + current_height, position_type(view.x), position_type(view.y) });

        current_height += view.y;
    }
}

#endif