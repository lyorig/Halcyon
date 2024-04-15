#pragma once

#include <SDL_image.h>

#include <halcyon/surface.hpp>

// image.hpp:
// Halcyon's image loading functionality.

namespace hal::image
{
    enum class format : lyo::u8
    {
        jpg  = IMG_INIT_JPG,
        png  = IMG_INIT_PNG,
        tif  = IMG_INIT_TIF,
        webp = IMG_INIT_WEBP
    };

    // Loads the necessary libraries for image loading.
    class context
    {
    public:
        context(std::initializer_list<format> types);
        ~context();

        context(const context&) = delete;

        static bool initialized();
    };
}