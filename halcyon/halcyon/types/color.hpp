#pragma once

#include <SDL2/SDL_pixels.h>

#include <algorithm>
#include <lyoSTL/cast.hpp>
#include <lyoSTL/types.hpp>

// color.hpp:
// An extension of SDL_Color with custom constructors,
// functions and operators.

namespace hal
{
    struct color : public SDL_Color
    {
        // A hex value in the form of 0xNNNNNN.
        using hexRGB = lyo::u32;

        // A single R, G, B or A value.
        using value = Uint8;

        // Predefined colors in RGB form.
        enum predefined : hexRGB
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

        constexpr color(hexRGB rgb, value alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { static_cast<value>((rgb >> 16) & 0xFF),
                static_cast<value>((rgb >> 8) & 0xFF),
                static_cast<value>(rgb & 0xFF),
                alpha }
        {
        }

        constexpr color(value red, value green, value blue,
            value alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { red, green, blue, alpha }
        {
        }

        constexpr SDL_Color to_sdl_color() const
        {
            return static_cast<SDL_Color>(*this);
        }
    };
} // namespace hal