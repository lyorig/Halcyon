#pragma once

#include <string_view>

#include <halcyon/internal/subsystem.hpp>

#include <halcyon/types/render.hpp>
#include <halcyon/utility/pass_key.hpp>

// video/display.hpp:
// A representation of a monitor.

namespace hal
{
    namespace proxy
    {
        class display;
    }

    namespace sdl
    {
        class display;
    }

    class display
    {
    public:
        using id_t = hal::u8;
        using hz_t = hal::u16;

        display() = default;
        display(const sdl::display& src);

        pixel_point size;
        hz_t        hz;
    };

    namespace sdl
    {
        // Display device data.
        class display : private SDL_DisplayMode
        {
        public:
            display() = default;
            display(hal::display::id_t id, pass_key<proxy::display>);
            display(const hal::display& src);

            pixel_point        size() const;
            hal::display::hz_t hz() const;
        };

        static_assert(sizeof(display) == sizeof(SDL_DisplayMode));
    }

}