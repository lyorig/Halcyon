#pragma once

#include <SDL_image.h>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/surface.hpp>

// image.hpp:
// SDL_image wrappers for image loading.

namespace hal::image
{
    enum class format
    {
        jpg  = IMG_INIT_JPG,
        png  = IMG_INIT_PNG,
        tif  = IMG_INIT_TIF,
        webp = IMG_INIT_WEBP,
        jxl  = IMG_INIT_JXL,
        avif = IMG_INIT_AVIF
    };

    enum class query_format : u8
    {
        avif,
        ico,
        cur,
        bmp,
        gif,
        jpg,
        jxl,
        lbm,
        pcx,
        png,
        pnm,
        svg,
        qoi,
        tif,
        xcf,
        xpm,
        xv,
        webp,

        unknown
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

        // Image loading functions.
        [[nodiscard]] surface load(accessor data) const;

        // Check an image's format.
        // This sets the accessor back to where it started, so const ref it is.
        query_format query(const accessor& data) const;

        static bool initialized();
    };

    static_assert(std::is_empty_v<context>);

    // Ensure calling debug::last_error() gives accurate information.
    static_assert(::IMG_GetError == ::SDL_GetError);
}