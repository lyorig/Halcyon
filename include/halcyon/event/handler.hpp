#pragma once

#include <halcyon/event/types.hpp>

// event/handler.hpp:
// A thin wrapper of SDL's event handling system.

namespace hal
{
    class events;

    namespace event
    {
        // Top-level event types.
        enum class type : u16
        {
            quit_requested = SDL_QUIT,
            terminated     = SDL_APP_TERMINATING,

            low_memory = SDL_APP_LOWMEMORY,

            will_enter_background = SDL_APP_WILLENTERBACKGROUND,
            entered_background    = SDL_APP_DIDENTERBACKGROUND,

            will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
            entered_foreground    = SDL_APP_DIDENTERFOREGROUND,

            display = SDL_DISPLAYEVENT,
            window  = SDL_WINDOWEVENT,

            key_pressed  = SDL_KEYDOWN,
            key_released = SDL_KEYUP,

            mouse_moved       = SDL_MOUSEMOTION,
            mouse_pressed     = SDL_MOUSEBUTTONDOWN,
            mouse_released    = SDL_MOUSEBUTTONUP,
            mouse_wheel_moved = SDL_MOUSEWHEEL,

            clipboard_updated = SDL_CLIPBOARDUPDATE
        };

        class handler
        {
        public:
            using authority = detail::subsystem<detail::system::events>;

            // Constructor that disables unused event.
            // This should reduce heap allocations on SDL's part.
            handler(authority&);

            // Get an event from the event queue.
            // Returns true if the polled event is valid, false if there are no more to process.
            bool poll();

            // Get the last polled event's type.
            type event_type() const;

            // Valid for: display
            const event::display& display() const;

            // Valid for: window
            const event::window& window() const;

            // Valid for: key_pressed, key_released
            const event::keyboard& keyboard() const;

            // Valid for: mouse_moved
            const event::mouse_motion& mouse_motion() const;

            // Valid for: mouse_pressed, mouse_released
            const event::mouse_button& mouse_button() const;

            // Valid for: mouse_wheel_moved
            const event::mouse_wheel& mouse_wheel() const;

            // Check whether there are any pending event in the event queue.
            static bool pending();

        private:
            // A union that impersonates SDL_Event.
            struct
            {
                union
                {
                    std::uint32_t type;

                    SDL_CommonEvent common;

                    event::display  display;
                    event::window   window;
                    event::keyboard key;

                    event::mouse_motion motion;
                    event::mouse_button button;
                    event::mouse_wheel  wheel;
                } data;

                std::byte padding[sizeof(SDL_Event) - sizeof(data)];
            } m_event;

            static_assert(sizeof(m_event) == sizeof(SDL_Event));
        };
    }

    std::string_view to_string(event::type evt);
}