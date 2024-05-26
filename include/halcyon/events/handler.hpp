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
        class display : SDL_DisplayEvent
        {
        public:
            enum class type : u8
            {
                connected    = SDL_DISPLAYEVENT_CONNECTED,
                disconnected = SDL_DISPLAYEVENT_DISCONNECTED,
                moved        = SDL_DISPLAYEVENT_MOVED,
                reoriented   = SDL_DISPLAYEVENT_ORIENTATION
            };

            display() = delete;

            type     event_type() const;
            display& event_type(type t);

            hal::display::id_t display_index() const;
            display&            display_index(hal::display::id_t);

            // As of April 2024, there are no events using the "data1" member.
        };

        static_assert(sizeof(display) == sizeof(SDL_DisplayEvent));

        class window : SDL_WindowEvent
        {
        public:
            enum class type : u8
            {
                shown               = SDL_WINDOWEVENT_SHOWN,
                hidden              = SDL_WINDOWEVENT_HIDDEN,
                exposed             = SDL_WINDOWEVENT_EXPOSED,
                moved               = SDL_WINDOWEVENT_MOVED,
                resized             = SDL_WINDOWEVENT_RESIZED,      // A window change caused by user interaction. Preceded by size_changed.
                size_changed        = SDL_WINDOWEVENT_SIZE_CHANGED, // A window change caused by an API call.
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

            window() = delete;

            type    event_type() const;
            window& event_type(type t);

            hal::window::id_t window_id() const;
            window&           window_id(hal::window::id_t id);

            // Valid for: display_changed.
            hal::display::id_t display_index() const;

            // This function automatically sets the event type to display_changed.
            window& display_index(hal::display::id_t id);

            // Valid for: resized, size_changed, moved.
            pixel_point point() const;

            // The provided type must be resized, size_changed, or moved.
            // This function automatically sets the event type.
            window& point(pixel_point pt, type t);
        };

        static_assert(sizeof(window) == sizeof(SDL_WindowEvent));

        class keyboard : SDL_KeyboardEvent
        {
        public:
            keyboard() = delete;

            hal::window::id_t window_id() const;
            keyboard&         window_id(hal::window::id_t id);

            hal::keyboard::button button() const;
            keyboard&             button(hal::keyboard::button btn);

            hal::keyboard::key key() const;
            keyboard&          key(hal::keyboard::key k);

            bool      repeat() const;
            keyboard& repeat(bool r);
        };

        static_assert(sizeof(keyboard) == sizeof(SDL_KeyboardEvent));

        class mouse_motion : SDL_MouseMotionEvent
        {
        public:
            mouse_motion() = delete;

            hal::window::id_t window_id() const;
            mouse_motion&     window_id(hal::window::id_t id);

            mouse::state  state() const;
            mouse_motion& state(mouse::state s);

            pixel_point   pos() const;
            mouse_motion& pos(pixel_point p);

            pixel_point   rel() const;
            mouse_motion& rel(pixel_point rel);
        };

        static_assert(sizeof(mouse_motion) == sizeof(SDL_MouseMotionEvent));

        class mouse_button : SDL_MouseButtonEvent
        {
        public:
            mouse_button() = delete;

            hal::window::id_t window_id() const;
            mouse_button&     window_id(hal::window::id_t id);

            mouse::button button() const;
            mouse_button& button(mouse::button btn);

            u8            click_amount() const;
            mouse_button& click_amount(u8 amnt);

            pixel_point   pos() const;
            mouse_button& pos(pixel_point pt);
        };

        static_assert(sizeof(mouse_button) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel : SDL_MouseWheelEvent
        {
        public:
            mouse_wheel() = delete;

            hal::window::id_t window_id() const;
            mouse_wheel&      window_id(hal::window::id_t id);

            pixel_point  pos() const;
            mouse_wheel& pos(pixel_point p);

            point<i16>   scroll() const;
            mouse_wheel& scroll(point<i16> s);

            point<f32>   scroll_precise() const;
            mouse_wheel& scroll_precise(point<f32> s);

            bool         scroll_flipped() const;
            mouse_wheel& scroll_flipped(bool f);
        };

        static_assert(sizeof(mouse_wheel) == sizeof(SDL_MouseWheelEvent));

        class text_input : SDL_TextInputEvent
        {
        public:
            // The maximum string length this event can handle.
            static constexpr std::size_t max_size()
            {
                return meta::array_size_v<decltype(SDL_TextInputEvent::text)> - 1;
            }

            hal::window::id_t window_id() const;
            text_input&       window_id(hal::window::id_t id);

            std::string_view text() const;
            text_input&      text(std::string_view t);
        };

        static_assert(sizeof(text_input) == sizeof(SDL_TextInputEvent));

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
            explicit handler(authority&);

            // Get an event from the event queue.
            // Returns true if the polled event is valid, false if there are no more to process.
            bool poll();

            // Push this event into the event queue.
            void push();

            // Start receiving text_input events.
            void text_input_start();

            // Stop receiving text_input events.
            void text_input_stop();

            // Get/set this event's current type.
            // Setting is only useful for events which carry no important data;
            // otherwise, accessing events directly does this automatically.
            type event_type() const;
            void event_type(type t);

            // Valid for: display
            const event::display& display() const;
            event::display&       display();

            // Valid for: window
            const event::window& window() const;
            event::window&       window();

            // Valid for: key_pressed, key_released
            const event::keyboard& keyboard() const;
            event::keyboard&       keyboard();

            // Valid for: text_input
            const event::text_input& text_input() const;
            event::text_input&       text_input();

            // Valid for: mouse_moved
            const event::mouse_motion& mouse_motion() const;
            event::mouse_motion&       mouse_motion();

            // Valid for: mouse_pressed, mouse_released
            const event::mouse_button& mouse_button() const;
            event::mouse_button&       mouse_button();

            // Valid for: mouse_wheel_moved
            const event::mouse_wheel& mouse_wheel() const;
            event::mouse_wheel&       mouse_wheel();

            // Check whether there are any pending event in the event queue.
            bool pending() const;

        private:
            void disable_unused();

            // A union that impersonates SDL_Event.
            struct
            {
                union
                {
                    std::uint32_t m_type;

                    SDL_CommonEvent m_common;

                    event::display  m_display;
                    event::window   m_window;
                    event::keyboard m_key;

                    event::text_input m_textInput;

                    event::mouse_motion m_motion;
                    event::mouse_button m_button;
                    event::mouse_wheel  m_wheel;
                } m_data;

                std::byte padding[sizeof(SDL_Event) - sizeof(m_data)] {};
            } m_event;

            static_assert(sizeof(m_event) == sizeof(SDL_Event));
        };
    }

    std::string_view to_string(event::type evt);
    std::string_view to_string(event::display::type evt);
    std::string_view to_string(event::window::type evt);
}