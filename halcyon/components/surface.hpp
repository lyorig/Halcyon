#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

/* surface.hpp:
   A "software surface" used by SDL. The user should never
   directly create a surface, but instead use various loading
   functions of other classes, which are responsible for providing
   a valid SDL_Surface pointer. On top of staying true to the SDL object
   structure, another upside comes in the form of early error handling. */

namespace hal
{
    class image_loader;
    class font;

    class surface : public sdl_object<SDL_Surface, &::SDL_FreeSurface>
    {
        friend class image_loader;
        friend class font;

        surface(SDL_Surface* surf) noexcept;

      public:

        pixel_size size() const noexcept;
    };
}  // namespace hal