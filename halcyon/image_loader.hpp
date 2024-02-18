#pragma once

#include <SDL_image.h>
#include <initializer_list>
#include <lyo/types.hpp>
#include <string_view>

namespace hal
{
    class engine;
    class surface;

    // For loading images, obviously. This class doesn't need to
    // outlive its loaded surface, so it may be used as an rvalue.
    class image_loader
    {
    public:
        enum image_type : lyo::u8 // Image types, adapted from SDL.
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP
        };

        image_loader(std::initializer_list<image_type> types);
        ~image_loader();

        static surface load(std::string_view file);

        static bool initialized();
    };
} // namespace hal