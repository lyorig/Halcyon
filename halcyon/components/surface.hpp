#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "font.hpp"
#include "sdl_object.hpp"

namespace hal
{
    class image_loader;
    class font;
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