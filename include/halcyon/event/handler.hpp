#pragma once

#include <halcyon/event/keyboard.hpp>
#include <halcyon/event/mouse.hpp>

#include <halcyon/internal/subsystem.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/window.hpp>

// event/event.hpp:
// A thin wrapper of SDL's event handling system.

namespace hal
{
    namespace events
    {
        class display_event : SDL_DisplayEvent
        {
        public:
            enum class type : u8
            {
                connected    = SDL_DISPLAYEVENT_CONNECTED,
                disconnected = SDL_DISPLAYEVENT_DISCONNECTED,
                moved        = SDL_DISPLAYEVENT_MOVED,
                reoriented   = SDL_DISPLAYEVENT_ORIENTATION
            };

            display_event() = delete;

            type event_type() const;

            hal::display::id_t display_index() const;

            // As of April 2024, there are no events using the "data1" member.
        };

        static_assert(sizeof(display_event) == sizeof(SDL_DisplayEvent));

        class window_event : SDL_WindowEvent
        {
        public:
            enum class type : u8
            {
                shown               = SDL_WINDOWEVENT_SHOWN,
                hidden              = SDL_WINDOWEVENT_HIDDEN,
                exposed             = SDL_WINDOWEVENT_EXPOSED,
                moved               = SDL_WINDOWEVENT_MOVED,
                resized             = SDL_WINDOWEVENT_SIZE_CHANGED, // The actual SDL resized event generates a duplicate event.
                minimized           = SDL_WINDOWEVENT_MINIMIZED,
                maximized           = SDL_WINDOWEVENT_MAXIMIZED,
                restored            = SDL_WINDOWEVENT_RESTORED,
                got_mouse_focus     = SDL_WINDOWEVENT_ENTER,
                lost_mouse_focus    = SDL_WINDOWEVENT_LEAVE,
                got_keyboard_focus  = SDL_WINDOWEVENT_FOCUS_GAINED,
                lost_keyboard_focus = SDL_WINDOWEVENT_FOCUS_LOST,
                closed              = SDL_WINDOWEVENT_CLOSE,
                focus_offered       = SDL_WINDOWEVENT_TAKE_FOCUS,
                hit_test            = SDL_WINDOWEVENT_HIT_TEST,
                icc_profile_changed = SDL_WINDOWEVENT_ICCPROF_CHANGED,
                display_changed     = SDL_WINDOWEVENT_DISPLAY_CHANGED
            };

            window_event() = delete;

            type event_type() const;

            hal::window::id_t window_id() const;

            // Valid for: display_changed
            hal::display::id_t new_display_index() const;

            // Valid for: resized, moved
            pixel_point new_point() const;
        };

        static_assert(sizeof(window_event) == sizeof(SDL_WindowEvent));

        class keyboard_event : SDL_KeyboardEvent
        {
        public:
            keyboard_event() = delete;

            hal::window::id_t window_id() const;

            hal::keyboard::button button() const;
            hal::keyboard::key    key() const;
            bool                  repeat() const;
        };

        static_assert(sizeof(keyboard_event) == sizeof(SDL_KeyboardEvent));

        class mouse_motion_event : SDL_MouseMotionEvent
        {
        public:
            mouse_motion_event() = delete;

            hal::window::id_t window_id() const;

            mouse::state state() const;
            pixel_point  pos() const;
            pixel_point  rel() const;
        };

        static_assert(sizeof(mouse_motion_event) == sizeof(SDL_MouseMotionEvent));

        class mouse_button_event : SDL_MouseButtonEvent
        {
        public:
            mouse_button_event() = delete;

            hal::window::id_t window_id() const;

            mouse::button button() const;
            u8            click_amount() const;
            pixel_point   pos() const;
        };

        static_assert(sizeof(mouse_button_event) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel_event : SDL_MouseWheelEvent
        {
        public:
            mouse_wheel_event() = delete;

            hal::window::id_t window_id() const;

            point<i16>  scroll() const;
            point<f32>  scroll_precise() const;
            pixel_point pos() const;
            bool        scroll_flipped() const;
        };

        static_assert(sizeof(mouse_wheel_event) == sizeof(SDL_MouseWheelEvent));
    }

    class event_handler
    {
    public:
        using authority = detail::subsystem<detail::system::events>;

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

        // Constructor that disables unused event.
        // This should reduce heap allocations on SDL's part.
        event_handler(authority&);

        // Get an event from the event queue.
        // Returns true if the polled event is valid, false if there are no more to process.
        bool poll();

        // Get the last polled event's type.
        type event_type() const;

        // Valid for: display
        const events::display_event& display() const;

        // Valid for: window
        const events::window_event& window() const;

        // Valid for: key_pressed, key_released
        const events::keyboard_event& keyboard() const;

        // Valid for: mouse_moved
        const events::mouse_motion_event& mouse_motion() const;

        // Valid for: mouse_pressed, mouse_released
        const events::mouse_button_event& mouse_button() const;

        // Valid for: mouse_wheel_moved
        const events::mouse_wheel_event& mouse_wheel() const;

        // Check whether there are any pending event in the event queue.
        static bool pending();

    private:
        // A union that impersonates SDL_Event.
        struct
        {
            union
            {
                std::uint32_t m_type;

                SDL_CommonEvent m_common;

                events::display_event  m_display;
                events::window_event   m_window;
                events::keyboard_event m_key;

                events::mouse_motion_event m_motion;
                events::mouse_button_event m_button;
                events::mouse_wheel_event  m_wheel;
            } m_data;

            std::byte padding[sizeof(SDL_Event) - sizeof(m_data)] {};
        } m_event;

        static_assert(sizeof(m_event) == sizeof(SDL_Event));
    };

    std::string_view to_string(event_handler::type evt);
    std::string_view to_string(events::display_event::type evt);
    std::string_view to_string(events::window_event::type evt);
}