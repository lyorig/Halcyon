#pragma once

#include <SDL2/SDL_surface.h>

#include <halcyon/types.hpp>

#include "sdl_object.hpp"

namespace hal
{
    using color_t = lyo::u32;

    class image_loader;
    class ttf_engine;
    class font;

    class surface : public sdl_object<SDL_Surface, ::SDL_FreeSurface>
    {
        friend class image_loader;
        friend class ttf_engine;

      public:

        pixel_size size() const noexcept;

      private:

        /* [image_loader] Image load constructor. */
        surface(const char* path) noexcept;

        /* [ttf_engine]   Text constructor. */
        surface(const font& fnt, const char* text) noexcept;
        surface(const font& fnt, const char* text, color_t color) noexcept;
    };
}  // namespace halcyon