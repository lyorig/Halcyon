#pragma once

#include <halcyon/events/keyboard.hpp>
#include <halcyon/events/mouse.hpp>

#include <halcyon/internal/subsystem.hpp>

#include <halcyon/video/display.hpp>
#include <halcyon/video/window.hpp>

// event/events.hpp:
// A thin wrapper of SDL's event handling system.

namespace hal
{
    namespace event
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

            type           event_type() const;
            display_event& event_type(type t);

            display::id_t  display_index() const;
            display_event& display_index(display::id_t);

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
                resized             = SDL_WINDOWEVENT_RESIZED, // The actual SDL resized event generates a duplicate event.
                size_changed        = SDL_WINDOWEVENT_SIZE_CHANGED,
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

            type          event_type() const;
            window_event& event_type(type t);

            window::id_t  window_id() const;
            window_event& window_id(window::id_t id);

            // Valid for: display_changed.
            display::id_t display_index() const;

            // This function automatically sets the event type to display_changed.
            window_event& display_index(display::id_t id);

            // Valid for: resized, moved
            pixel_point point() const;

            // The provided type must be either resized or moved.
            // This function automatically sets the event type.
            window_event& point(pixel_point pt, type t);
        };

        static_assert(sizeof(window_event) == sizeof(SDL_WindowEvent));

        class keyboard_event : SDL_KeyboardEvent
        {
        public:
            keyboard_event() = delete;

            window::id_t    window_id() const;
            keyboard_event& window_id(window::id_t id);

            keyboard::button button() const;
            keyboard_event&  button(keyboard::button btn);

            keyboard::key   key() const;
            keyboard_event& key(keyboard::key k);

            bool            repeat() const;
            keyboard_event& repeat(bool r);
        };

        static_assert(sizeof(keyboard_event) == sizeof(SDL_KeyboardEvent));

        class mouse_motion_event : SDL_MouseMotionEvent
        {
        public:
            mouse_motion_event() = delete;

            window::id_t        window_id() const;
            mouse_motion_event& window_id(window::id_t id);

            mouse::state        state() const;
            mouse_motion_event& state(mouse::state s);

            pixel_point         pos() const;
            mouse_motion_event& pos(pixel_point p);

            pixel_point         rel() const;
            mouse_motion_event& rel(pixel_point rel);
        };

        static_assert(sizeof(mouse_motion_event) == sizeof(SDL_MouseMotionEvent));

        class mouse_button_event : SDL_MouseButtonEvent
        {
        public:
            mouse_button_event() = delete;

            window::id_t        window_id() const;
            mouse_button_event& window_id(window::id_t id);

            mouse::button       button() const;
            mouse_button_event& button(mouse::button btn);

            u8                  click_amount() const;
            mouse_button_event& click_amount(u8 amnt);

            pixel_point         pos() const;
            mouse_button_event& pos(pixel_point pt);
        };

        static_assert(sizeof(mouse_button_event) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel_event : SDL_MouseWheelEvent
        {
        public:
            mouse_wheel_event() = delete;

            window::id_t       window_id() const;
            mouse_wheel_event& window_id(window::id_t id);

            pixel_point        pos() const;
            mouse_wheel_event& pos(pixel_point p);

            point<i16>         scroll() const;
            mouse_wheel_event& scroll(point<i16> s);

            point<f32>         scroll_precise() const;
            mouse_wheel_event& scroll_precise(point<f32> s);

            bool               scroll_flipped() const;
            mouse_wheel_event& scroll_flipped(bool f);
        };

        static_assert(sizeof(mouse_wheel_event) == sizeof(SDL_MouseWheelEvent));

        class text_input_event : SDL_TextInputEvent
        {
        public:
            // The maximum string length this event can handle.
            static constexpr std::size_t max_size()
            {
                return meta::array_size_v<decltype(SDL_TextInputEvent::text)> - 1;
            }

            window::id_t      window_id() const;
            text_input_event& window_id(window::id_t id);

            std::string_view  text() const;
            text_input_event& text(std::string_view t);
        };

        static_assert(sizeof(text_input_event) == sizeof(SDL_TextInputEvent));

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

            text_input = SDL_TEXTINPUT,

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

            // Constructor that disables unused events.
            // This should reduce heap allocations on SDL's part.
            handler(authority&);

            // Get an event from the event queue.
            // Returns true if the polled event is valid, false if there are no more to process.
            bool poll();

            // Push this event into the event queue.
            void push();

            // Get/set this event's current type.
            // Setting is only useful for events which carry no important data;
            // otherwise, accessing events directly does this automatically.
            type event_type() const;
            void event_type(type t);

            // Valid for: display
            const display_event& display() const;
            display_event&       display();

            // Valid for: window
            const window_event& window() const;
            window_event&       window();

            // Valid for: key_pressed, key_released
            const keyboard_event& keyboard() const;
            keyboard_event&       keyboard();

            // Valid for: text_input
            const text_input_event& text_input() const;
            text_input_event&       text_input();

            // Valid for: mouse_moved
            const mouse_motion_event& mouse_motion() const;
            mouse_motion_event&       mouse_motion();

            // Valid for: mouse_pressed, mouse_released
            const mouse_button_event& mouse_button() const;
            mouse_button_event&       mouse_button();

            // Valid for: mouse_wheel_moved
            const mouse_wheel_event& mouse_wheel() const;
            mouse_wheel_event&       mouse_wheel();

            // Check whether there are any pending event in the event queue.
            bool pending() const;

        private:
            // A union that impersonates SDL_Event.
            struct
            {
                union
                {
                    std::uint32_t m_type;

                    SDL_CommonEvent m_common;

                    display_event  m_display;
                    window_event   m_window;
                    keyboard_event m_key;

                    text_input_event m_textInput;

                    mouse_motion_event m_motion;
                    mouse_button_event m_button;
                    mouse_wheel_event  m_wheel;
                } m_data;

                std::byte padding[sizeof(SDL_Event) - sizeof(m_data)] {};
            } m_event;

            static_assert(sizeof(m_event) == sizeof(SDL_Event));
        };
    }

    std::string_view to_string(event::type evt);
    std::string_view to_string(event::display_event::type evt);
    std::string_view to_string(event::window_event::type evt);
}