#pragma once

#include <SDL2/SDL_image.h>

#include "components/surface.hpp"

namespace hal
{
    class window;

    class image_loader
    {
      public:

        enum image_type : lyo::u8  // Image types, adapted from SDL.
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP
        };

        image_loader(window& wnd, int type_flags) noexcept;
        ~image_loader();

        surface load_image(const char* file) const noexcept;

      private:
    };
}  // namespace hal