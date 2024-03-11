#pragma once

#include <SDL_video.h>

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <string_view>

// display.hpp:
// A representation of a monitor.

namespace hal
{
    class display
    {
    public:
        using index_t = lyo::u8;
        using hz_t    = lyo::u16;

        display(index_t disp_idx);
        display(index_t disp_idx, index_t mode_idx);

        static index_t amount();

        index_t num_modes() const;

        index_t          idx() const;
        pixel_point      size() const;
        std::string_view name() const;
        hz_t             hz() const;

        // This is implemented as a standalone function in SDL, for if you
        // just want the name without the overhead of getting all other display info.
        static std::string_view name(index_t idx);

    private:
        hal::pixel_point m_size;

        hz_t    m_hz;
        index_t m_index;
    };
}