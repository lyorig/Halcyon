#pragma once

#include <SDL_image.h>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/surface.hpp>

// images.hpp:
// Halcyon's image loading functionality.

namespace hal::image
{
    enum format : lyo::u8
    {
        jpg  = IMG_INIT_JPG,
        png  = IMG_INIT_PNG,
        tif  = IMG_INIT_TIF,
        webp = IMG_INIT_WEBP
    };

    // Loads the necessary libraries for image loading. Use as a "fire-and-forget" object,
    // i.e. create it, and load from anywhere afterwards.
    class cleanup
    {
    public:
        cleanup(std::initializer_list<format> types);
        ~cleanup();

        static bool initialized();
    };

    // Image loading is not dependent on a specific object, so it's a free function.
    surface load(accessor data);
}