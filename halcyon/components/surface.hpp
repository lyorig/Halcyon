#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types/types.hpp>
#include <lyo/pass_key.hpp>

#include "font.hpp"
#include "sdl_object.hpp"

namespace hal
{
    class image_loader;
    class font;

    enum color : color_t
    {
        black = 0x000000,

        red   = 0xFF0000,
        green = 0x00FF00,
        blue  = 0x0000FF,

        orange = 0xFFAC1C,

        white = 0xFFFFFF
    };

    class surface : public sdl_object<SDL_Surface, ::SDL_FreeSurface>
    {
      public:

        pixel_size size() const noexcept;

        /* Text constructor. */
        surface(const font& fnt, const char* text, color clr = white) noexcept;

        /* Image loader constructor. */
        surface(const char* path, lyo::pass_key<image_loader>) noexcept;

      private:

        static constexpr SDL_Color convert_color(color clr) noexcept;
    };
}  // namespace hal