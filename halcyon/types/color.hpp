#pragma once

#include <SDL2/SDL_pixels.h>

#include <lyo/types.hpp>

namespace hal
{
    struct color_base
    {
        using hex_type   = lyo::u32;
        using value_type = lyo::u8;
    };

    enum class color : color_base::hex_type
    {
        black = 0x000000,

        red   = 0xFF0000,
        green = 0x00FF00,
        blue  = 0x0000FF,

        orange = 0xFFFF00,
        cyan   = 0x00FFFF,

        white = 0xFFFFFF
    };

    struct rgb : public color_base
    {
        constexpr rgb(hex_type color) noexcept :
            r { static_cast<value_type>((color >> 16) & 0xFF) },
            g { static_cast<value_type>((color >> 8) & 0xFF) },
            b { static_cast<value_type>(color & 0xFF) }
        {
        }

        constexpr rgb(color clr) noexcept :
            rgb { static_cast<hex_type>(clr) }
        {
        }

        constexpr SDL_Color to_color() const noexcept
        {
            return SDL_Color {
                r,
                g,
                b,
                255
            };
        }

        value_type r, g, b;
    };

    TAG_TYPE(no_alpha);

    struct rgba : public rgb
    {
        constexpr rgba(hex_type color) noexcept :
            rgb { color >> 8 },  // Strip the alpha value.
            a { static_cast<value_type>(color & 0xFF) }
        {
        }

        constexpr rgba(color clr) noexcept :
            rgb { clr },
            a { SDL_ALPHA_OPAQUE }
        {
        }

        constexpr SDL_Color to_color() const noexcept
        {
            return SDL_Color {
                r,
                g,
                b,
                a
            };
        }

        value_type a;
    };
}