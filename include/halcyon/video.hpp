#pragma once

#include <halcyon/context.hpp>

#include <halcyon/image.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/ttf.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/renderer.hpp>
#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/subsystem.hpp>

namespace hal
{
    class video : public detail::subinit<system::video>
    {
        class clipboard_proxy
        {
        public:
            // Get current clipboard text.
            // This has to be freed, so a std::string is provided instead.
            std::string operator()() const;

            // Set clipboard text.
            void operator()(std::string_view text);

            bool has_text() const;
        };

        static_assert(std::is_empty_v<clipboard_proxy>);

        class display_proxy
        {
        public:
            display::index_t size() const;

            display operator[](display::index_t idx) const;
        };

        static_assert(std::is_empty_v<display_proxy>);

    public:
        explicit video(context& auth);

        LYO_NOSIZE detail::subsystem<system::events> events;

        LYO_NOSIZE clipboard_proxy clipboard;
        LYO_NOSIZE display_proxy   displays;
    };

    static_assert(std::is_empty_v<video>);
}