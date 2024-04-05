#pragma once

#include <SDL_image.h>
#include <lyo/types.hpp>

#include <halcyon/internal/accessor.hpp>

namespace hal
{
    class surface;

    // Loads the necessary libraries for image loading. Use as a "fire-and-forget" object,
    // i.e. create it, and load from anywhere afterwards.
    class image_cleanup
    {
    public:
        enum format : lyo::u8 // Image types, adapted from SDL.
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP
        };

        image_cleanup(std::initializer_list<format> types);
        ~image_cleanup();

        static bool initialized();
    };

    // Image loading is not dependent on a specific object, so it's a free function.
    surface load_image(accessor data);

} // namespace hal