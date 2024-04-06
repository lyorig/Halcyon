#pragma once

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <halcyon/enums/buttons.hpp>
#include <halcyon/other/display.hpp>
#include <halcyon/window.hpp>
#include <span>

// events.hpp:
// A thin wrapper of SDL's event handling system.
// As there are many various events that would be quite time-consuming to adapt
// (or am I just lazy?), a zero-overhead wrapper is provided instead.

namespace hal
{
    class event
    {
    public:
        // Specific event classes.
        class display_event
        {
        public:
            using event_id_t = lyo::u8;
            using payload_t  = lyo::i32;

            enum type : lyo::u8
            {
                connected    = SDL_DISPLAYEVENT_CONNECTED,
                disconnected = SDL_DISPLAYEVENT_DISCONNECTED,
                moved        = SDL_DISPLAYEVENT_MOVED,
                reoriented   = SDL_DISPLAYEVENT_ORIENTATION
            };

            display_event(const SDL_DisplayEvent& evt)
                : type { evt.event }
                , display_index { static_cast<display::index_t>(evt.display) }
                , data { evt.data1 }
            {
            }

            enum type        type;
            display::index_t display_index;
            payload_t        data;
        };

        class window_event
        {
        public:
            using payload_t = lyo::i32;

            enum type : lyo::u8
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

            window_event(const SDL_WindowEvent& evt)
                : window_id { static_cast<window::id_t>(evt.windowID) }
                , type { evt.event }
                , data { evt.data1, evt.data2 }
            {
            }

            window::id_t window_id;
            enum type    type;

            std::pair<payload_t, payload_t> data;
        };

        class keyboard_event
        {
            keyboard_event(const SDL_KeyboardEvent& evt);

            window::id_t window_id;
            button       button;
            bool         repeat;
        };

        class mouse_motion_event
        {
        };

        class mouse_button_event
        {
        public:
            mouse_button_event(const SDL_MouseButtonEvent& evt)
                : window_id { static_cast<window::id_t>(evt.windowID) }
                , button { evt.button }
                , click_amount { evt.clicks }
                , pos { static_cast<pixel_t>(evt.x), static_cast<pixel_t>(evt.y) }
            {
            }

            window::id_t window_id;
            button_mouse button;
            lyo::u8      click_amount;
            pixel_point  pos;
        };

        class mouse_wheel_event
        {
        public:
            mouse_wheel_event(const SDL_MouseWheelEvent& evt)
                : scroll { evt.preciseX, evt.preciseY }
                , pos { static_cast<pixel_t>(evt.mouseX), static_cast<pixel_t>(evt.mouseY) }
                , window_id { static_cast<window::id_t>(evt.windowID) }
                , scroll_flipped { evt.direction == SDL_MOUSEWHEEL_FLIPPED }
            {
            }

            sdl::coord_point scroll;
            pixel_point      pos;
            window::id_t     window_id;
            bool             scroll_flipped;
        };

        using tick_t    = lyo::u32;
        using variant_t = std::variant<display_event, window_event, keyboard_event, mouse_motion_event, mouse_wheel_event, mouse_wheel_event>;

        enum type : lyo::u16
        {
            quit_requested = SDL_QUIT,
            terminated     = SDL_APP_TERMINATING,

            low_memory = SDL_APP_LOWMEMORY,

            will_enter_background = SDL_APP_WILLENTERBACKGROUND,
            entered_background    = SDL_APP_DIDENTERBACKGROUND,

            will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
            entered_foreground    = SDL_APP_DIDENTERFOREGROUND,

            // locale_changed = SDL_LOCALECHANGED,

            display_event = SDL_DISPLAYEVENT,
            window_event  = SDL_WINDOWEVENT,

            key_pressed  = SDL_KEYDOWN,
            key_released = SDL_KEYUP,

            // text_editing   = SDL_TEXTEDITING,
            // text_input     = SDL_TEXTINPUT,
            // keymap_changed = SDL_KEYMAPCHANGED,

            mouse_motion   = SDL_MOUSEMOTION,
            mouse_pressed  = SDL_MOUSEBUTTONDOWN,
            mouse_released = SDL_MOUSEBUTTONUP,
            mouse_wheel    = SDL_MOUSEWHEEL,

            // user_event = SDL_USEREVENT
        };

        // Get an event from the event queue.
        bool poll();

        // Push this event into the event queue.
        void push();

        // Check whether there are any pending events in the event queue.
        static bool pending();

        // Get the current keyboard state. Index with hal::button values.
        static std::span<const std::uint8_t> keyboard_state();

    private:

        SDL_Event m_event;
        variant_t m_data;
    };
}