#pragma once

#include <SDL_events.h>

#include <halcyon/keyboard.hpp>
#include <halcyon/mouse.hpp>

#include <halcyon/video/window.hpp>

// event/types.hpp:
// Event type wrappers for SDL. Their sizes must match their
// SDL counterparts, as Halcyon does some union hackery to
// speed things up. Extensive static_assert checking is recommended.

namespace hal
{
    namespace event
    {
        class handler;

        class display : SDL_DisplayEvent
        {
        public:
            enum class type : lyo::u8
            {
                connected    = SDL_DISPLAYEVENT_CONNECTED,
                disconnected = SDL_DISPLAYEVENT_DISCONNECTED,
                moved        = SDL_DISPLAYEVENT_MOVED,
                reoriented   = SDL_DISPLAYEVENT_ORIENTATION
            };

            display(const SDL_DisplayEvent& evt, lyo::pass_key<event::handler>);

            type type() const;

            hal::display::index_t display_index() const;

            // As of April 2024, there are no events using the "data1" member.
        };

        static_assert(sizeof(display) == sizeof(SDL_DisplayEvent));

        class window : SDL_WindowEvent
        {
        public:
            enum class type : lyo::u8
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

            type type() const;

            hal::window::id_t window_id() const;

            // Valid for: display_changed
            hal::display::index_t new_display_index() const;

            // Valid for: resized, moved
            pixel_point new_point() const;
        };

        static_assert(sizeof(window) == sizeof(SDL_WindowEvent));

        class keyboard : SDL_KeyboardEvent
        {
        public:
            hal::window::id_t window_id() const;

            hal::keyboard::button button() const;
            hal::keyboard::key    key() const;
            bool                  repeat() const;
        };

        static_assert(sizeof(keyboard) == sizeof(SDL_KeyboardEvent));

        class mouse_motion : SDL_MouseMotionEvent
        {
        public:
            hal::window::id_t window_id() const;

            mouse::state state() const;
            pixel_point  pos() const;
            pixel_point  rel() const;
        };

        static_assert(sizeof(mouse_motion) == sizeof(SDL_MouseMotionEvent));

        class mouse_button : SDL_MouseButtonEvent
        {
        public:
            hal::window::id_t window_id() const;

            mouse::state  state() const;
            mouse::button button() const;
            lyo::u8       click_amount() const;
            pixel_point   pos() const;
        };

        static_assert(sizeof(mouse_button) == sizeof(SDL_MouseButtonEvent));

        class mouse_wheel : SDL_MouseWheelEvent
        {
        public:
            hal::window::id_t window_id() const;

            point<lyo::i16> scroll() const;
            point<lyo::f32> scroll_precise() const;
            pixel_point     pos() const;
            bool            scroll_flipped() const;
        };

        static_assert(sizeof(mouse_wheel) == sizeof(SDL_MouseWheelEvent));
    }
}