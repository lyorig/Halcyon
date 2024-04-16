#pragma once

#include <initializer_list>

#include <SDL_image.h>

#include <halcyon/surface.hpp>

// image.hpp:
// Halcyon's image loading functionality.

namespace hal
{
    class surface;

    namespace image
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
            // Initialize the image loader context with chosen types.
            context(std::initializer_list<format> types);

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Convenience image loading functions.
            [[nodiscard]] surface load(accessor data) const;

            static bool initialized();
        };
    }

    // Ensure calling debug::last_error() gives accurate information.
    static_assert(::IMG_GetError == ::SDL_GetError);
}