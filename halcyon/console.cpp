#ifndef NDEBUG

    #include "console.hpp"

    #include "components/font.hpp"
    #include "texture.hpp"
    #include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};

void console::draw(const font& fnt, const window& wnd) noexcept
{
    constexpr coordinate offset { 20.0, 20.0 };

    int w, h;

    ::TTF_SizeUTF8(fnt.ptr(), "", &w, &h);

    pixel_t height { 0 };

    for (const auto& entry : m_queue)
    {
        texture {
            wnd, fnt.render(entry.first, static_cast<color>(entry.second))
        }
            .draw({ offset.x, offset.y + static_cast<double>(height) });

        height += h;
    }
}

#endif