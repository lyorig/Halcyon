#pragma once

#include <SDL_events.h>

#include <halcyon/other/display.hpp>
#include <halcyon/other/keyboard_mouse.hpp>
#include <halcyon/window.hpp>

// event_types.hpp:
// Event type wrappers for SDL. Their sizes must match their
// SDL counterparts, as Halcyon does some union hackery to
// speed things up. Extensive static_assert checking is recommended.

namespace hal::events
{
    class display : SDL_DisplayEvent
    {
    public:
        using event_id_t = lyo::u8;
        using payload_t  = lyo::i32;

        enum event_type : lyo::u8
        {
            connected    = SDL_DISPLAYEVENT_CONNECTED,
            disconnected = SDL_DISPLAYEVENT_DISCONNECTED,
            moved        = SDL_DISPLAYEVENT_MOVED,
            reoriented   = SDL_DISPLAYEVENT_ORIENTATION
        };

        event_type            type() const;
        hal::display::index_t display_index() const;
        payload_t             data() const;
    };

    static_assert(sizeof(display) == sizeof(SDL_DisplayEvent));

    class window : SDL_WindowEvent
    {
    public:
        using payload_t = lyo::i32;

        enum event_type : lyo::u8
        {
            shown               = SDL_WINDOWEVENT_SHOWN,
            hidden              = SDL_WINDOWEVENT_HIDDEN,
            exposed             = SDL_WINDOWEVENT_EXPOSED,
            moved               = SDL_WINDOWEVENT_MOVED,
            resized             = SDL_WINDOWEVENT_RESIZED,
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

        hal::window::id_t window_id() const;

        event_type type() const;

        std::pair<payload_t, payload_t> data() const;
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

        point<lyo::f32> scroll() const;
        pixel_point     pos() const;
        bool            scroll_flipped() const;
    };

    static_assert(sizeof(mouse_wheel) == sizeof(SDL_MouseWheelEvent));
}