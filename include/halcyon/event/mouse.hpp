#pragma once

#include <string_view>

#include <SDL3/SDL_mouse.h>

#include <halcyon/types/render.hpp>
#include <halcyon/utility/pass_key.hpp>

#include <halcyon/internal/subsystem.hpp>

// event/mouse.hpp:
// Mouse button access and data.

namespace hal
{
    // Forward declarations for mouse::state's pass-key constructors.
    namespace event
    {
        class mouse_motion;
        class mouse_button;
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
            using authority = hal::detail::subsystem<hal::detail::system::events>;

            // Default constructor that captures the mouse state at the time of construction.
            state(pass_key<authority>);

            // Private constructors meant for events.
            state(std::uint32_t mask, pass_key<event::mouse_motion>);

            bool operator[](button btn) const;

        private:
            u8 m_state;
        };
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