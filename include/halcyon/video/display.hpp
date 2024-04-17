#pragma once

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <string_view>

// video/display.hpp:
// A representation of a monitor.

namespace hal::video
{
    class system;

    // Display device data.
    class display
    {
    public:
        using id_t = lyo::u8;
        using hz_t = lyo::u16;

        display(id_t disp_idx, lyo::pass_key<system>);

        pixel_point size() const;
        hz_t        hz() const;
        id_t        index() const;

        std::string_view name() const;

    private:
        pixel_point m_size;
        hz_t        m_hz;
        id_t        m_index;
    };
}