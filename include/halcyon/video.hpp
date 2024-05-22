#pragma once

#include <halcyon/context.hpp>

#include <halcyon/image.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/ttf.hpp>

#include <halcyon/events.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/driver.hpp>
#include <halcyon/video/message_box.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/string.hpp>
#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    namespace proxy
    {
        using video = detail::subsystem<detail::system::video>;

        class clipboard
        {
        public:
            using authority_t = video;

            clipboard(pass_key<authority_t>);

            // Get current clipboard text.
            // This has to be freed, so a custom wrapper is returned.
            sdl::string text() const;

            // Set clipboard text.
            void text(std::string_view text);

            bool has_text() const;
        };

        class display
        {
        public:
            using authority_t = video;

            display(pass_key<authority_t>);

            hal::display::id_t size() const;

            hal::display operator[](hal::display::id_t idx) const;
        };
    }

    HAL_TAG(fullscreen);

    namespace detail
    {
        template <>
        class subsystem<system::video>
        {
            using event_proxy = subsystem<system::events>;

        public:
            using parent_t = subinit<system::video>;

            subsystem(pass_key<parent_t>);

            // Create a window.
            [[nodiscard]] window make_window(std::string_view title, pixel_point size, std::initializer_list<window::flags> flags = {}) &;

            // Warning: This has some issues on macOS due to its DPI scaling stuff.
            [[nodiscard]] window make_window(std::string_view title, HAL_TAG_NAME(fullscreen)) &;

            HAL_NO_SIZE proxy::events events;

            HAL_NO_SIZE proxy::clipboard clipboard;
            HAL_NO_SIZE proxy::display displays;

        private:
            subsystem();
        };
    }
}