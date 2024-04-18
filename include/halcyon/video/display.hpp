#pragma once

#include <string_view>

#include <halcyon/types/render.hpp>
#include <halcyon/utility/pass_key.hpp>

// video/display.hpp:
// A representation of a monitor.

namespace hal::video
{
    class system;

    // Display device data.
    class display
    {
    public:
        using id_t = u8;
        using hz_t = u16;

        // [private] Display info is provided by the display proxy in hal::video::system.
        display(id_t disp_idx, pass_key<system>);

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