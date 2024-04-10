#pragma once

#include <SDL_pixels.h>

#include <algorithm>
#include <lyo/cast.hpp>
#include <lyo/types.hpp>

// color.hpp:
// An extension of SDL_Color with custom constructors,
// functions and operators.

namespace hal
{
    struct color : public SDL_Color
    {
        // A hex value in the form of 0xNNNNNN.
        using hex_t = lyo::u32;

        // A single R, G, B or A value.
        using value_t = Uint8;

        // Predefined colors in RGB form.
        enum predefined : hex_t
        {
            black = 0x000000,

            red   = 0xFF0000,
            green = 0x00FF00,
            blue  = 0x0000FF,

            yellow = 0xFFFF00,
            orange = 0xFFA500,
            cyan   = 0x00FFFF,

            weezer_blue = 0x0099D3,

            white = 0xFFFFFF
        };

        enum limits
        {
            min = SDL_ALPHA_TRANSPARENT,
            max = SDL_ALPHA_OPAQUE
        };

        constexpr color()
            : color { black }
        {
        }

        constexpr color(hex_t rgb, value_t alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { static_cast<value_t>((rgb >> 16) & 0xFF),
                static_cast<value_t>((rgb >> 8) & 0xFF),
                static_cast<value_t>(rgb & 0xFF),
                alpha }
        {
        }

        constexpr color(value_t red, value_t green, value_t blue,
            value_t alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { red, green, blue, alpha }
        {
        }

        constexpr SDL_Color to_sdl_color() const
        {
            return static_cast<SDL_Color>(*this);
        }
    };
} // namespace hal