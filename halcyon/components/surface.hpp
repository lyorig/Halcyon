#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

/* surface.hpp:
   A "software surface" used by SDL. The user should never
   directly create a surface, but instead use various loading
   functions of other classes. */

namespace hal
{
    class surface : public sdl_object<SDL_Surface, ::SDL_FreeSurface>
    {
        // In this instance, friend classes are simpler than a pass_key.
        friend class image_loader;
        friend class font;

      public:

        pixel_size size() const noexcept;

      private:

        surface(SDL_Surface* surf) noexcept;
    };
}  // namespace hal