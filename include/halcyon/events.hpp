#pragma once

#include <halcyon/other/display.hpp>
#include <halcyon/types/events.hpp>
#include <halcyon/window.hpp>

// events.hpp:
// A thin wrapper of SDL's event handling system.

namespace hal
{
    // Top-level event types.
    enum class event : lyo::u16
    {
        quit_requested = SDL_QUIT,
        terminated     = SDL_APP_TERMINATING,

        low_memory = SDL_APP_LOWMEMORY,

        will_enter_background = SDL_APP_WILLENTERBACKGROUND,
        entered_background    = SDL_APP_DIDENTERBACKGROUND,

        will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
        entered_foreground    = SDL_APP_DIDENTERFOREGROUND,

        display_event = SDL_DISPLAYEVENT,
        window_event  = SDL_WINDOWEVENT,

        key_pressed  = SDL_KEYDOWN,
        key_released = SDL_KEYUP,

        mouse_moved       = SDL_MOUSEMOTION,
        mouse_pressed     = SDL_MOUSEBUTTONDOWN,
        mouse_released    = SDL_MOUSEBUTTONUP,
        mouse_wheel_moved = SDL_MOUSEWHEEL,

        clipboard_updated = SDL_CLIPBOARDUPDATE
    };

    class event_handler
    {
    public:
        // Constructor that disables unused events.
        // This should reduce heap allocations on SDL's part.
        event_handler();

        // Get an event from the event queue.
        // Returns true if the polled event is valid, false if there are no more to process.
        bool poll();

        // Get the last polled event's type.
        event type() const;

        // Valid for: display
        const events::display& display() const;

        // Valid for: window
        const events::window& window() const;

        // Valid for: key_pressed, key_released
        const events::keyboard& keyboard() const;

        // Valid for: mouse_moved
        const events::mouse_motion& mouse_motion() const;

        // Valid for: mouse_pressed, mouse_released
        const events::mouse_button& mouse_button() const;

        // Valid for: mouse_wheel_moved
        const events::mouse_wheel& mouse_wheel() const;

        // Check whether there are any pending events in the event queue.
        static bool pending();

    private:
        // A union that impersonates SDL_Event.
        struct
        {
            union
            {
                std::uint32_t type;

                SDL_CommonEvent common;

                events::display  display;
                events::window   window;
                events::keyboard key;

                events::mouse_motion motion;
                events::mouse_button button;
                events::mouse_wheel  wheel;
            } data;

            std::byte padding[sizeof(SDL_Event) - sizeof(data)];
        } m_event;

        static_assert(sizeof(m_event) == sizeof(SDL_Event));
    };
}