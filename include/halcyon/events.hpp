#pragma once

#include <halcyon/events/holder.hpp>

#include <halcyon/internal/subsystem.hpp>

// events.hpp:
// Master include file for Halcyon Events, plus the events subsystem.

namespace hal
{
    namespace proxy
    {
        using events = detail::subsystem<detail::system::events>;

        class mouse
        {
        public:
            using authority_t = events;

            mouse(pass_key<authority_t>);

            // Get a snapshot of the current mouse state.
            hal::mouse::state state() const;

            // Get the current mouse state relative to the desktop.
            pixel::point pos_abs() const;

            // Get the current mouse state relative to the focus window.
            pixel::point pos_rel() const;
        };

        class keyboard
        {
        public:
            using authority_t = events;

            keyboard(pass_key<authority_t>);

            // Get a reference to the keyboard state.
            hal::keyboard::state_reference state_ref() const;
            hal::keyboard::mod_state       mod() const;
        };
    }

    namespace detail
    {
        template <>
        class subsystem<system::events>
        {
        public:
            using authority_t = subsystem<system::video>;
            using parent_t    = subinit<system::events>;

            subsystem(pass_key<authority_t>);
            subsystem(pass_key<parent_t>);

            bool poll(event::holder& eh);

            void push(const event::holder& eh);

            bool pending();

            void text_input_start();
            void text_input_stop();

            HAL_NO_SIZE proxy::mouse mouse;
            HAL_NO_SIZE proxy::keyboard keyboard;

        private:
            // [private] Delegating constructor.
            subsystem();
        };
    }
}