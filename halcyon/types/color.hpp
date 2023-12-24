#pragma once

#include <SDL2/SDL_pixels.h>

#include <algorithm>
#include <lyo/cast.hpp>
#include <lyo/types.hpp>

namespace hal
{
    struct color : public SDL_Color
    {
        using hex_type = lyo::u32;
        using value_type = Uint8;

        // Predefined colors in RGB form.
        enum predefined : hex_type
        {
            black = 0x000000,

            red = 0xFF0000,
            green = 0x00FF00,
            blue = 0x0000FF,

            yellow = 0xFFFF00,
            orange = 0xFFA500,
            cyan = 0x00FFFF,

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

        constexpr color(hex_type rgb, value_type alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { static_cast<value_type>((rgb >> 16) & 0xFF),
                static_cast<value_type>((rgb >> 8) & 0xFF),
                static_cast<value_type>(rgb & 0xFF),
                static_cast<value_type>(rgb) }
        {
        }

        constexpr color(value_type red, value_type green, value_type blue,
            value_type alpha = SDL_ALPHA_OPAQUE)
            : SDL_Color { red, green, blue, alpha }
        {
        }

        constexpr SDL_Color to_sdl_color() const
        {
            return static_cast<SDL_Color>(*this);
        }
    };
} // namespace hal