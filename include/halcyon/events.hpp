#pragma once

#include <halcyon/events/handler.hpp>

#include <halcyon/internal/subsystem.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace detail
    {
        class mouse_proxy
        {
        public:
            using authority_t = subsystem<system::events>;

            mouse_proxy(pass_key<authority_t>);

            // Get a snapshot of the current mouse state.
            mouse::state state() const;

            // Get the current mouse state relative to the desktop.
            hal::pixel_point pos_abs() const;

            // Get the current mouse state relative to the focus window.
            hal::pixel_point pos_rel() const;
        };

        class keyboard_proxy
        {
        public:
            using authority_t = subsystem<system::events>;

            keyboard_proxy(pass_key<authority_t>);

            // Get a reference to the keyboard state.
            keyboard::state_reference state_ref() const;
        };

        template <>
        class subsystem<system::events>
        {
        public:
            using authority_t = subsystem<system::video>;
            using parent_t    = subinit<system::events>;

            subsystem(pass_key<authority_t>);
            subsystem(pass_key<parent_t>);

            HAL_NO_SIZE mouse_proxy    mouse;
            HAL_NO_SIZE keyboard_proxy keyboard;

        private:
            subsystem();
        };
    }

    namespace system
    {
        using events = detail::subinit<detail::system::events>;
    }
}