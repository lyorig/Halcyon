#pragma once

#include <string_view>

#include <SDL_mouse.h>

#include <halcyon/video/types.hpp>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/internal/subsystem.hpp>

// event/mouse.hpp:
// Mouse button access and data.

namespace hal
{
    namespace event
    {
        class mouse_motion;
    }

    namespace proxy
    {
        class mouse;
    }

    namespace mouse
    {
        enum class button : u8
        {
            left   = SDL_BUTTON_LEFT,
            middle = SDL_BUTTON_MIDDLE,
            right  = SDL_BUTTON_RIGHT,
            x1     = SDL_BUTTON_X1,
            x2     = SDL_BUTTON_X2,
        };

        // A snapshot of the current mouse state. Unlike the keyboard state reference,
        // you need to recreate this object to observe any changes.
        class state
        {
        public:
            using authority_t = proxy::mouse;

            // [private] A snapshot of the current mouse state is provided by the event subsystem.
            state(pass_key<authority_t>);

            // [private] Constructor meant for events.
            state(std::uint32_t mask, pass_key<event::mouse_motion>);

            u8 mask() const;

            bool operator[](button btn) const;

        private:
            u8 m_state;
        };

        pixel_point pos_rel();
        pixel_point pos_abs();
    }

    constexpr std::string_view to_string(mouse::button btn)
    {
        using enum hal::mouse::button;

        switch (btn)
        {
        case left:
            return "Left Mouse";

        case right:
            return "Right Mouse";

        case middle:
            return "Middle Mouse";

        case x1:
            return "Mouse Extra 1";

        case x2:
            return "Mouse Extra 2";
        }
    }
}