#pragma once

#include <string_view>

#include <SDL3/SDL_video.h>

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
        using id_t = detail::cft<u8, SDL_DisplayID>;
        using hz_t = u16;

        // [private] Display info is provided by the display proxy in hal::video::system.
        display(id_t disp_idx, pass_key<system>);

        pixel_point size() const;
        hz_t        hz() const;
        id_t        index() const;

        std::string_view name() const;

    private:
        const SDL_DisplayMode* m_ptr;
    };
}