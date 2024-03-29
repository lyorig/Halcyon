#pragma once

#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <halcyon/enums/buttons.hpp>
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
        enum type
        {
            quit_requested = SDL_QUIT,
            terminated     = SDL_APP_TERMINATING,

            low_memory = SDL_APP_LOWMEMORY,

            will_enter_background = SDL_APP_WILLENTERBACKGROUND,
            entered_background    = SDL_APP_DIDENTERBACKGROUND,

            will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
            entered_foreground    = SDL_APP_DIDENTERFOREGROUND,

            locale_changed = SDL_LOCALECHANGED,

            display_event = SDL_DISPLAYEVENT,
            window_event  = SDL_WINDOWEVENT,

            key_pressed  = SDL_KEYDOWN,
            key_released = SDL_KEYUP,

            text_editing   = SDL_TEXTEDITING,
            text_input     = SDL_TEXTINPUT,
            keymap_changed = SDL_KEYMAPCHANGED,

            mouse_motion   = SDL_MOUSEMOTION,
            mouse_pressed  = SDL_MOUSEBUTTONDOWN,
            mouse_released = SDL_MOUSEBUTTONUP,
            mouse_wheel    = SDL_MOUSEWHEEL,

            user_event = SDL_USEREVENT
        };

        // Get an event from the event queue.
        bool poll();

        // Push this event into the event queue.
        void push();

        // Check whether there are any pending events in the event queue.
        static bool pending();

        static std::span<const Uint8> keyboard_state();

        static bool state(type t);
        static void state(type t, bool enable);

        SDL_Event data;
    };

    namespace window_event
    {
        enum type
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
    }
}