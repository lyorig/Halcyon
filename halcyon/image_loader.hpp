#pragma once

#include <SDL2/SDL_image.h>

#include <halcyon/types/other.hpp>

namespace hal
{
    class video;
    class surface;

    // For loading images, obviously. This class doesn't need to
    // outlive its loaded surface, so it may be used as an rvalue.
    class image_loader
    {
    public:
        enum image_type : lyo::u8 // Image types, adapted from SDL.
        {
            jpg = IMG_INIT_JPG,
            png = IMG_INIT_PNG,
            tif = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP
        };

        image_loader(video& sys, il<image_type> types);
        ~image_loader();

        surface load(const char* file) const;
    };
} // namespace hal