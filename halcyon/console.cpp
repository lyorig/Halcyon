#ifndef NDEBUG

    #include "console.hpp"

    #include "texture.hpp"

using namespace hal;

console::queue_type console::m_queue {};

void console::draw(const font& fnt, const window& wnd) noexcept
{
    int w, h;

    // The resulting surface's height is constant.
    ::TTF_SizeUTF8(fnt.ptr(), "", &w, &h);

    pixel_t height { 0 };

    for (const auto& pr : m_queue)
    {
        texture {
            wnd, {fnt, pr.first.c_str(), static_cast<color>(pr.second)}
        }.draw({ 0.0, static_cast<double>(height) });

        height += h;
    }
}

#endif