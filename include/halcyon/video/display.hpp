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

    namespace display
    {
        using id_t = u8;
        using hz_t = hal::u16;
    }

    namespace info
    {
        namespace sdl
        {
            class display;
        }

        class display
        {
        public:
            display() = default;
            display(const sdl::display& src);

            pixel_point        size;
            pixel_format       fmt;
            hal::display::hz_t hz;
        };

        namespace sdl
        {
            // Display device data.
            class display : private SDL_DisplayMode
            {
            public:
                display() = default;
                display(hal::display::id_t id, pass_key<proxy::display>);
                display(const info::display& src);

                pixel_point        size() const;
                pixel_format       format() const;
                hal::display::hz_t hz() const;
            };

            static_assert(sizeof(display) == sizeof(SDL_DisplayMode));
        }
    }
}