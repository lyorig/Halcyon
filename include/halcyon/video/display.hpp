#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <string_view>

// video/display.hpp:
// A representation of a monitor.

namespace hal
{
    class video;

    // Display device data.
    class display
    {
    public:
        using index_t = lyo::u8;
        using hz_t    = lyo::u16;

        display(index_t disp_idx, lyo::pass_key<video>);

        pixel_point size() const;
        hz_t        hz() const;
        index_t     index() const;

        std::string_view name() const;

    private:
        pixel_point m_size;
        hz_t        m_hz;
        index_t     m_index;
    };
}