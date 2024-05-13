#pragma once

#include <algorithm>
#include <limits> // MSVC wants this.

#include <SDL_pixels.h>

#include <halcyon/types/numeric.hpp>

// types/color.hpp:
// An extension of SDL_Color with custom constructors,
// functions and operators.

namespace hal
{
    struct color : public SDL_Color
    {
        // A hex value in the form of 0xNNNNNN.
        using hex_t = u32;

        // A difference between two colors.
        using diff_t = i16;

        // A single R, G, B or A value.
        using value_t = std::uint8_t;

        // Helper struct that represents the difference of two colors.
        struct diff
        {
            diff_t r, g, b, a;

            constexpr friend diff operator*(diff d, f64 mul)
            {
                return { static_cast<diff_t>(d.r * mul), static_cast<diff_t>(d.g * mul), static_cast<diff_t>(d.b * mul), static_cast<diff_t>(d.a * mul) };
            }
        };

        constexpr color()
            : color { 0x000000 }
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

        // Negate this color.
        constexpr color operator-() const
        {
            constexpr value_t max { std::numeric_limits<value_t>::max() };
            return { static_cast<value_t>(max - r), static_cast<value_t>(max - g), static_cast<value_t>(max - b), a };
        }

        constexpr friend bool operator==(color a, color b)
        {
            return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
        }

        constexpr friend diff operator-(color a, color b)
        {
            return { static_cast<diff_t>(a.r - b.r), static_cast<diff_t>(a.g - b.g), static_cast<diff_t>(a.b - b.b), static_cast<diff_t>(a.a - b.a) };
        }

        constexpr friend color operator+(color a, color b)
        {
            constexpr int max { std::numeric_limits<value_t>::max() };

            return {
                static_cast<value_t>(std::min(a.r + b.r, max)),
                static_cast<value_t>(std::min(a.g + b.g, max)),
                static_cast<value_t>(std::min(a.b + b.b, max)),
                a.a
            };
        }

        constexpr friend color operator+(color a, diff b)
        {
            return {
                static_cast<value_t>(a.r + b.r),
                static_cast<value_t>(a.g + b.g),
                static_cast<value_t>(a.b + b.b),
                static_cast<value_t>(a.a + b.a),
            };
        }

        constexpr friend color operator-(color a, diff b)
        {
            return {
                static_cast<value_t>(a.r - b.r),
                static_cast<value_t>(a.g - b.g),
                static_cast<value_t>(a.b - b.b),
                static_cast<value_t>(a.a - b.a),
            };
        }
    };

    // Predefined colors in RGB form.
    namespace palette
    {
        constexpr color::hex_t black       = 0x000000,
                               red         = 0xFF0000,
                               green       = 0x00FF00,
                               blue        = 0x0000FF,
                               yellow      = 0xFFFF00,
                               orange      = 0xFFA500,
                               cyan        = 0x00FFFF,
                               weezer_blue = 0x0099D3,
                               white       = 0xFFFFFF;
    }
}