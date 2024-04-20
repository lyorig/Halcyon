#pragma once

#include <string_view>

#include <SDL3/SDL_mouse.h>

#include <halcyon/types/render.hpp>
#include <halcyon/utility/pass_key.hpp>

// mouse.hpp:
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
            // Default constructor that captures the mouse state at the time of construction.
            state();

            // Private constructors meant for events.
            state(std::uint32_t mask, pass_key<event::mouse_motion>);
            state(std::uint32_t mask, pass_key<event::mouse_button>);

            bool operator[](button btn) const;

        private:
            // Delegated to by pass-key constructors.
            state(std::uint32_t mask);

            u8 m_state;
        };

        coord_point pos_rel();
        coord_point pos_abs();
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