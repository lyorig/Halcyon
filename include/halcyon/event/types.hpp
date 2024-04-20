#pragma once

#include <SDL3/SDL_events.h>

#include <halcyon/event/keyboard.hpp>
#include <halcyon/event/mouse.hpp>

#include <halcyon/video/window.hpp>

// event/types.hpp:
// Event type wrappers for SDL. Their sizes must match their
// SDL counterparts, as Halcyon does some union hackery to
// speed things up. Extensive static_assert checking is recommended.

namespace hal
{
    namespace event
    {
        class display : SDL_DisplayEvent
        {
        public:
            // TODO: Finish this.
            enum class type : u16
            {
                connected    = SDL_EVENT_DISPLAY_ADDED,
                disconnected = SDL_EVENT_DISPLAY_REMOVED,
                moved        = SDL_EVENT_DISPLAY_MOVED,
                reoriented   = SDL_EVENT_DISPLAY_ORIENTATION
            };

            type event_type() const;

            hal::video::display::id_t display_index() const;

            // As of April 2024, there are no events using the "data1" member.
        };

        static_assert(sizeof(display) == sizeof(SDL_DisplayEvent));

        class window : SDL_WindowEvent
        {
        public:
            // TODO: Finish this.
            enum class type : u16
            {
                shown               = SDL_EVENT_WINDOW_SHOWN,
                hidden              = SDL_EVENT_WINDOW_HIDDEN,
                exposed             = SDL_EVENT_WINDOW_EXPOSED,
                moved               = SDL_EVENT_WINDOW_MOVED,
                resized             = SDL_EVENT_WINDOW_RESIZED, // The actual SDL resized event generates a duplicate event.
                minimized           = SDL_EVENT_WINDOW_MINIMIZED,
                maximized           = SDL_EVENT_WINDOW_MAXIMIZED,
                restored            = SDL_EVENT_WINDOW_RESTORED,
                got_focus           = SDL_EVENT_WINDOW_FOCUS_GAINED,
                lost_focus          = SDL_EVENT_WINDOW_FOCUS_LOST,
                lost_keyboard_focus = SDL_EVENT_WINDOW_FOCUS_LOST,
                close_requested     = SDL_EVENT_WINDOW_CLOSE_REQUESTED,
                focus_offered       = SDL_EVENT_WINDOW_TAKE_FOCUS,
                hit_test            = SDL_EVENT_WINDOW_HIT_TEST,
                icc_profile_changed = SDL_EVENT_WINDOW_ICCPROF_CHANGED,
                display_changed     = SDL_EVENT_WINDOW_DISPLAY_CHANGED
            };

            type event_type() const;

            hal::video::window::id_t window_id() const;

            // Valid for: display_changed
            hal::video::display::id_t new_display_index() const;

            // Valid for: resized, moved
            pixel_point new_point() const;
        };

        static_assert(sizeof(window) == sizeof(SDL_WindowEvent));

        class keyboard : SDL_KeyboardEvent
        {
        public:
            hal::video::window::id_t window_id() const;

            hal::keyboard::button button() const;
            hal::keyboard::key    key() const;
            bool                  repeat() const;
        };

        static_assert(sizeof(keyboard) == sizeof(SDL_KeyboardEvent));

        class mouse_motion : SDL_MouseMotionEvent
        {
        public:
            hal::video::window::id_t window_id() const;

            mouse::state state() const;
            pixel_point  pos() const;
            pixel_point  rel() const;
        };

        static_assert(sizeof(mouse_motion) == sizeof(SDL_MouseMotionEvent));

        class mouse_button : SDL_MouseButtonEvent
        {
        public:
            hal::video::window::id_t window_id() const;

            mouse::state  state() const;
            mouse::button button() const;
            u8            click_amount() const;
            pixel_point   pos() const;
        };

        static_assert(sizeof(mouse_button) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel : SDL_MouseWheelEvent
        {
        public:
            hal::video::window::id_t window_id() const;

            point<i16>  scroll() const;
            point<f32>  scroll_precise() const;
            pixel_point pos() const;
            bool        scroll_flipped() const;
        };

        static_assert(sizeof(mouse_wheel) == sizeof(SDL_MouseWheelEvent));
    }

    std::string_view to_string(event::display::type evt);
    std::string_view to_string(event::window::type evt);
}