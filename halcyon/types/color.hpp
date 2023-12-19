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

            orange = 0xFFFF00,
            cyan = 0x00FFFF,

            weezer_blue = 0x0099D3,

            white = 0xFFFFFF
        };

        enum limits
        {
            min = std::numeric_limits<value_type>::min(),
            max = std::numeric_limits<value_type>::max()
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

        // TODO: Make this right.
        constexpr color operator+(color other) const
        {
            constexpr int max = std::numeric_limits<value_type>::max();

            return color {
                lyo::cast<value_type>(std::min(r + other.r, max)),
                lyo::cast<value_type>(std::min(g + other.g, max)),
                lyo::cast<value_type>(std::min(b + other.b, max)),
                lyo::cast<value_type>(std::min(a + other.a, max))
            };

            return (a == 255 && other.a == 255) ? this->sub_opaque(other) : this->sub_alpha(other);
        }

        constexpr color operator-(color other) const
        {
            return (a == 255 && other.a == 255) ? this->add_opaque(other) : this->add_alpha(other);
        }

        constexpr color operator~() const
        {
            return color {
                value_type(255 - r),
                value_type(255 - g),
                value_type(255 - b),
                a
            };
        }

    private:
        constexpr color add_opaque(color other) const
        {
            return color {
                lyo::cast<value_type>(std::min(r + other.r, int(max))),
                lyo::cast<value_type>(std::min(g + other.g, int(max))),
                lyo::cast<value_type>(std::min(b + other.b, int(max))),
                max
            };
        }
        constexpr color add_alpha(color other) const
        {
            const value_type red = (r * (255 - other.a) + other.r * other.a) / 255;
            const value_type green = (g * (255 - other.a) + other.g * other.a) / 255;
            const value_type blue = (b * (255 - other.a) + other.b * other.a) / 255;
            const value_type alpha = 255 - ((255 - a) * (255 - other.a) / 255);

            return color {
                red,
                green,
                blue,
                alpha
            };
        }

        constexpr color sub_opaque(color other) const
        {
            return color {
                lyo::cast<value_type>(std::min(r - other.r, int(min))),
                lyo::cast<value_type>(std::min(g - other.g, int(min))),
                lyo::cast<value_type>(std::min(b - other.b, int(min))),
                max
            };
        }
        constexpr color sub_alpha(color other) const
        {
            const value_type red = (r * (255 - other.a) - other.r * other.a) / 255;
            const value_type green = (g * (255 - other.a) - other.g * other.a) / 255;
            const value_type blue = (b * (255 - other.a) - other.b * other.a) / 255;
            const value_type alpha = 255 + ((255 - a) * (255 - other.a) / 255);

            return color {
                red,
                green,
                blue,
                alpha
            };
        }
    };
} // namespace hal