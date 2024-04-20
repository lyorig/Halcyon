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
        enum class type : detail::cft<u16, std::underlying_type_t<SDL_EventType>>
        {
            quit_requested = SDL_EVENT_QUIT,
            terminated     = SDL_EVENT_TERMINATING,

            low_memory = SDL_EVENT_LOW_MEMORY,

            will_enter_background = SDL_EVENT_WILL_ENTER_BACKGROUND,
            entered_background    = SDL_EVENT_DID_ENTER_BACKGROUND,

            will_enter_foreground = SDL_EVENT_WILL_ENTER_FOREGROUND,
            entered_foreground    = SDL_EVENT_DID_ENTER_FOREGROUND,

            changed_locale       = SDL_EVENT_LOCALE_CHANGED,
            system_theme_changed = SDL_EVENT_SYSTEM_THEME_CHANGED,

            key_pressed  = SDL_EVENT_KEY_DOWN,
            key_released = SDL_EVENT_KEY_UP,

            mouse_moved       = SDL_EVENT_MOUSE_MOTION,
            mouse_pressed     = SDL_EVENT_MOUSE_BUTTON_DOWN,
            mouse_released    = SDL_EVENT_MOUSE_BUTTON_UP,
            mouse_wheel_moved = SDL_EVENT_MOUSE_WHEEL,
            mouse_added       = SDL_EVENT_MOUSE_ADDED,
            mouse_removed     = SDL_EVENT_MOUSE_REMOVED,

            // Display events.
            display_reoriented            = SDL_EVENT_DISPLAY_ORIENTATION,
            display_added                 = SDL_EVENT_DISPLAY_ADDED,
            display_removed               = SDL_EVENT_DISPLAY_REMOVED,
            display_moved                 = SDL_EVENT_DISPLAY_MOVED,
            display_content_scale_changed = SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED,
            display_hdr_state_changed     = SDL_EVENT_DISPLAY_HDR_STATE_CHANGED,

            // Window events.
            window_shown                 = SDL_EVENT_WINDOW_SHOWN,
            window_hidden                = SDL_EVENT_WINDOW_HIDDEN,
            window_exposed               = SDL_EVENT_WINDOW_EXPOSED,
            window_moved                 = SDL_EVENT_WINDOW_MOVED,
            window_resized               = SDL_EVENT_WINDOW_RESIZED,
            window_changed_pixel_size    = SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED,
            window_minimized             = SDL_EVENT_WINDOW_MINIMIZED,
            window_maximized             = SDL_EVENT_WINDOW_MAXIMIZED,
            window_restored              = SDL_EVENT_WINDOW_RESTORED,
            window_mouse_entered         = SDL_EVENT_WINDOW_MOUSE_ENTER,
            window_mouse_left            = SDL_EVENT_WINDOW_MOUSE_LEAVE,
            window_focus_gained          = SDL_EVENT_WINDOW_FOCUS_GAINED,
            window_focus_lost            = SDL_EVENT_WINDOW_FOCUS_LOST,
            window_close_requested       = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
            window_focus_offered         = SDL_EVENT_WINDOW_TAKE_FOCUS,
            window_hit_test              = SDL_EVENT_WINDOW_HIT_TEST,
            window_changed_icc_profile   = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
            window_changed_display       = SDL_EVENT_WINDOW_DISPLAY_CHANGED,
            window_changed_display_scale = SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED,
            window_occluded              = SDL_EVENT_WINDOW_OCCLUDED,
            window_fullscreen_entered    = SDL_EVENT_WINDOW_ENTER_FULLSCREEN,
            window_fullscreen_left       = SDL_EVENT_WINDOW_LEAVE_FULLSCREEN,
            window_destroyed             = SDL_EVENT_WINDOW_DESTROYED,

            audio_device_added          = SDL_EVENT_AUDIO_DEVICE_ADDED,
            audio_device_removed        = SDL_EVENT_AUDIO_DEVICE_REMOVED,
            audio_device_format_changed = SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED,

            clipboard_updated = SDL_EVENT_CLIPBOARD_UPDATE
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

                std::byte padding[sizeof(SDL_Event) - sizeof(data)] {};
            } m_event;

            static_assert(sizeof(m_event) == sizeof(SDL_Event));

            bool is_window_event() const;
            bool is_display_event() const;
        };
    }

    std::string_view to_string(event::type evt);
}