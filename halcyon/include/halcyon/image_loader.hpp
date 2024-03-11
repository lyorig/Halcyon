#pragma once

#include <SDL_image.h>
#include <lyo/types.hpp>
#include <string_view>

namespace hal
{
    class surface;

    // For loading images, obviously.
    class image_loader
    {
    public:
        enum format : lyo::u8 // Image types, adapted from SDL.
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP
        };

        image_loader(std::initializer_list<format> types);
        ~image_loader();

        static surface load(std::string_view file);

        static bool initialized();
    };
} // namespace hal