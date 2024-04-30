#pragma once

#include <SDL_image.h>

#include <halcyon/internal/accessor.hpp>
#include <halcyon/surface.hpp>

// image.hpp:
// SDL_image wrappers for image loading.

namespace hal

{
    namespace image
    {
        enum class init_format : u8
        {
            jpg  = IMG_INIT_JPG,
            png  = IMG_INIT_PNG,
            tif  = IMG_INIT_TIF,
            webp = IMG_INIT_WEBP,
            jxl  = IMG_INIT_JXL,
            avif = IMG_INIT_AVIF
        };

        enum class save_format : u8
        {
            png,
            jpg
        };

        enum class load_format : u8
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

        // Loads, and provides, image manipulation functionality.
        class context
        {
        public:
            // Initialize the image context with chosen types.
            context(std::initializer_list<init_format> types);

            context(const context&) = delete;
            context(context&&)      = delete;

            ~context();

            // Load an image, automatically deducing the format.
            [[nodiscard]] surface load(accessor src) const;

            // Load an image, knowing the format in advance.
            [[nodiscard]] surface load(accessor src, load_format fmt) const;

            // Save a surface with a specified format.
            // JPEG files are currently saved at a hard-coded 90 quality.
            void save(const surface& surf, save_format fmt, outputter dst) const;

            // Check an image's format.
            // This sets the accessor back to where it started, so const ref it is.
            load_format query(const accessor& src) const;

            static bool initialized();
        };

        static_assert(std::is_empty_v<context>);

        // Ensure calling debug::last_error() gives accurate information.
        static_assert(::IMG_GetError == ::SDL_GetError);
    }

    std::string_view to_string(image::init_format fmt);
}