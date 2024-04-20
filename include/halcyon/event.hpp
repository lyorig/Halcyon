#pragma once

#include <halcyon/event/handler.hpp>

#include <halcyon/internal/subsystem.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace detail
    {
        template <>
        class subsystem<system::events>
        {
            class mouse_proxy
            {
            public:
                // Get a snapshot of the current mouse state.
                mouse::state state() const;

                // Get the current mouse state relative to the desktop.
                hal::coord_point pos_abs() const;

                // Get the current mouse state relative to the focus window.
                hal::coord_point pos_rel() const;
            };

            class keyboard_proxy
            {
            public:
                // Get a reference to the keyboard state.
                keyboard::state_reference state_ref() const;
            };

        public:
            subsystem() = default;

            HAL_NO_SIZE mouse_proxy    mouse;
            HAL_NO_SIZE keyboard_proxy keyboard;
        };
    }

    namespace event
    {
        class system : public detail::subinit<detail::system::events>
        {
        public:
            using subinit::subinit;
        };

        static_assert(std::is_empty_v<system>);
    }
}