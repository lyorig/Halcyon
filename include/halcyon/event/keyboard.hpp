#pragma once

#include <string_view>

#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/types/render.hpp>

#include <halcyon/internal/subsystem.hpp>

// keyboard.hpp:
// Keyboard enumerations and access.

namespace hal
{
    namespace detail
    {
        class keyboard_proxy;
    }

    namespace keyboard
    {
        // A representation of a physical, layout- and locale-independent button.
        enum class button : detail::cft<u8, std::underlying_type_t<SDL_Scancode>>
        {
            A = SDL_SCANCODE_A,
            B = SDL_SCANCODE_B,
            C = SDL_SCANCODE_C,
            D = SDL_SCANCODE_D,
            E = SDL_SCANCODE_E,
            F = SDL_SCANCODE_F,
            G = SDL_SCANCODE_G,
            H = SDL_SCANCODE_H,
            I = SDL_SCANCODE_I,
            J = SDL_SCANCODE_J,
            K = SDL_SCANCODE_K,
            L = SDL_SCANCODE_L,
            M = SDL_SCANCODE_M,
            N = SDL_SCANCODE_N,
            O = SDL_SCANCODE_O,
            P = SDL_SCANCODE_P,
            Q = SDL_SCANCODE_Q,
            R = SDL_SCANCODE_R,
            S = SDL_SCANCODE_S,
            T = SDL_SCANCODE_T,
            U = SDL_SCANCODE_U,
            V = SDL_SCANCODE_V,
            W = SDL_SCANCODE_W,
            X = SDL_SCANCODE_X,
            Y = SDL_SCANCODE_Y,
            Z = SDL_SCANCODE_Z,

            one   = SDL_SCANCODE_1,
            two   = SDL_SCANCODE_2,
            three = SDL_SCANCODE_3,
            four  = SDL_SCANCODE_4,
            five  = SDL_SCANCODE_5,
            six   = SDL_SCANCODE_6,
            seven = SDL_SCANCODE_7,
            eight = SDL_SCANCODE_8,
            nine  = SDL_SCANCODE_9,

            F1  = SDL_SCANCODE_F1,
            F2  = SDL_SCANCODE_F2,
            F3  = SDL_SCANCODE_F3,
            F4  = SDL_SCANCODE_F4,
            F5  = SDL_SCANCODE_F5,
            F6  = SDL_SCANCODE_F6,
            F7  = SDL_SCANCODE_F7,
            F8  = SDL_SCANCODE_F8,
            F9  = SDL_SCANCODE_F9,
            F10 = SDL_SCANCODE_F10,
            F11 = SDL_SCANCODE_F11,
            F12 = SDL_SCANCODE_F12,

            esc        = SDL_SCANCODE_ESCAPE,
            tab        = SDL_SCANCODE_TAB,
            caps_lock  = SDL_SCANCODE_CAPSLOCK,
            left_shift = SDL_SCANCODE_LSHIFT,
            left_ctrl  = SDL_SCANCODE_LCTRL,

            backspace   = SDL_SCANCODE_BACKSPACE,
            enter       = SDL_SCANCODE_RETURN,
            right_shift = SDL_SCANCODE_RSHIFT,
            right_ctrl  = SDL_SCANCODE_RCTRL,

            left_alt  = SDL_SCANCODE_LALT,
            right_alt = SDL_SCANCODE_RALT,
        };

        // A representation of a locale- and layout-dependent key.
        enum class key : detail::cft<u32, std::underlying_type_t<SDL_KeyCode>>
        {
            A = SDLK_a,
            B = SDLK_b,
            C = SDLK_c,
            D = SDLK_d,
            E = SDLK_e,
            F = SDLK_f,
            G = SDLK_g,
            H = SDLK_h,
            I = SDLK_i,
            J = SDLK_j,
            K = SDLK_k,
            L = SDLK_l,
            M = SDLK_m,
            N = SDLK_n,
            O = SDLK_o,
            P = SDLK_p,
            Q = SDLK_q,
            R = SDLK_r,
            S = SDLK_s,
            T = SDLK_t,
            U = SDLK_u,
            V = SDLK_v,
            W = SDLK_w,
            X = SDLK_x,
            Y = SDLK_y,
            Z = SDLK_z,

            one   = SDLK_1,
            two   = SDLK_2,
            three = SDLK_3,
            four  = SDLK_4,
            five  = SDLK_5,
            six   = SDLK_6,
            seven = SDLK_7,
            eight = SDLK_8,
            nine  = SDLK_9,

            F1  = SDLK_F1,
            F2  = SDLK_F2,
            F3  = SDLK_F3,
            F4  = SDLK_F4,
            F5  = SDLK_F5,
            F6  = SDLK_F6,
            F7  = SDLK_F7,
            F8  = SDLK_F8,
            F9  = SDLK_F9,
            F10 = SDLK_F10,
            F11 = SDLK_F11,
            F12 = SDLK_F12,

            esc        = SDLK_ESCAPE,
            tab        = SDLK_TAB,
            caps_lock  = SDLK_CAPSLOCK,
            left_shift = SDLK_LSHIFT,
            left_ctrl  = SDLK_LCTRL,

            backspace   = SDLK_BACKSPACE,
            enter       = SDLK_RETURN,
            right_shift = SDLK_RSHIFT,
            right_ctrl  = SDLK_RCTRL,

            left_alt  = SDLK_LALT,
            right_alt = SDLK_RALT,
        };

        key    to_key(button btn);
        button to_button(key k);

        // A reference to the keyboard state. Unlike that mouse state,
        // you can keep this object around, as it always references the current
        // state as long as you keep polling for event in your application loop.
        class state_reference
        {
        public:
            using authority_t = detail::keyboard_proxy;

            state_reference(pass_key<authority_t>);

            bool operator[](button btn) const;
            bool operator[](key k) const;

        private:
            const std::uint8_t* m_arr;
        };
    }

    std::string_view to_string(keyboard::button btn);
    std::string_view to_string(keyboard::key k);
}