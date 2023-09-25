#pragma once

#include <SDL2/SDL_pixels.h>

#include <lyo/types.hpp>

namespace hal
{
    struct color : public SDL_Color
    {
        using hex_type   = lyo::u32;
        using value_type = lyo::u8;

        // Predefined colors in RGB form.
        enum predefined : hex_type
        {
            black = 0x000000,

            red   = 0xFF0000,
            green = 0x00FF00,
            blue  = 0x0000FF,

            orange = 0xFFFF00,
            cyan   = 0x00FFFF,

            white = 0xFFFFFF
        };

        constexpr color(hex_type rgba_value) noexcept :
            SDL_Color {
                static_cast<value_type>((rgba_value >> 24) & 0xFF),
                static_cast<value_type>((rgba_value >> 16) & 0xFF),
                static_cast<value_type>((rgba_value >> 8) & 0xFF),
                static_cast<value_type>(rgba_value & 0xFF)
            }
        {
        }

        constexpr color(value_type red, value_type green, value_type blue, value_type alpha = SDL_ALPHA_OPAQUE) noexcept :
            SDL_Color {
                red,
                green,
                blue,
                alpha
            }
        {
        }

        constexpr color(predefined clr) noexcept :
            color { (static_cast<hex_type>(clr) << 8) | 0xFF }
        {
        }

        constexpr SDL_Color to_sdl_color() const noexcept
        {
            return static_cast<SDL_Color>(*this);
        }
    };
}