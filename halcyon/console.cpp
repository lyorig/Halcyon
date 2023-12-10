#include "SDL2/SDL_stdinc.h"
#include "halcyon/debug.hpp"
#include "halcyon/internal/config.hpp"
#ifndef NDEBUG

#include "console.hpp"

#include "components/font.hpp"
#include "texture.hpp"
#include "window.hpp"

using namespace hal;

console::queue_type console::m_queue {};
console::count_type console::m_entries { 0 };

bool console::m_repaint { false };

void console::draw(const font& fnt, const window& wnd)
{
    // Render settings.
    constexpr coordinate offset { 20.0, 10.0 };
    constexpr lyo::f64 vhm { 4.0 / 100.0 }; // View-height multiplier (only edit the lhs).

    // I am not 100% sure whether it's safe to create a static
    // SDL object, as the destructor will run after de-initialization.
    // However, it doesn't crash, it's fast, and it's debug, so who cares.
    static texture tx { wnd };

    if (m_repaint) {
        static pixel_type y_size { fnt.render(".").size().y };

        pixel_size csz { .y = pixel_type(y_size * m_entries) }; // Canvas size.

        // "Calculate" canvas size.
        for (count_type i { 0 }; i < m_entries; ++i) {
            const pixel_type x_size { fnt.size_text(m_queue[i].first).x };
            if (x_size > csz.x)
                csz.x = x_size;
        }

        if (csz.x != 0) {
            // Compose the texture.
            const lyo::f64 scale { wnd.size().y * vhm / y_size };
            const pixel_type y_scaled { pixel_type(y_size * scale) };
            surface canvas { wnd, csz * scale };

            for (count_type i { 0 }; i < m_entries; ++i) {
                const value_pair& entry { m_queue[i] };
                if (!entry.first.empty()) {
                    const surface text { fnt.render(entry.first, color::hex_type(entry.second)) };

                    const pixel_type xsz { text.size().x };
                    const pixel_area dest {
                        0,
                        pixel_type(y_scaled * i),
                        pixel_type(xsz * scale),
                        pixel_type(y_scaled)
                    };

                    hal::surface::draw(text).to(dest)(canvas);
                }
            }

            tx = canvas;
        }

        m_repaint = false;
    }

    hal::texture::draw(tx).to(offset)();
}

#endif
