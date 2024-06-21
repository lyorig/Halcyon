#pragma once

#include <string_view>

#include <SDL_blendmode.h>
#include <SDL_pixels.h>

#include <halcyon/types/rectangle.hpp>

// types/render.hpp:
// Rendering-related types used throughout Halcyon.
// Defining HAL_FAST_TYPES maps Halcyon's types directly to SDL's where applicable.

namespace hal
{
    enum class blend_mode : u8
    {
        none  = SDL_BLENDMODE_NONE,
        add   = SDL_BLENDMODE_ADD,
        mod   = SDL_BLENDMODE_MOD,
        mul   = SDL_BLENDMODE_MUL,
        blend = SDL_BLENDMODE_BLEND
    };

    constexpr std::string_view to_string(blend_mode bm)
    {
        switch (bm)
        {
            using enum blend_mode;

        case none:
            return "None";

        case blend:
            return "Blend";

        case add:
            return "Add";

        case mod:
            return "Mod";

        case mul:
            return "Mul";

        default:
            return "[unknown]";
        }
    }

    namespace pixel
    {
        using point = point<pixel_t>;
        using rect  = rectangle<pixel_t>;

        // Formats in which pixels are stored.
        enum class format : u32
        {
            unknown = SDL_PIXELFORMAT_UNKNOWN,

            index1_lsb = SDL_PIXELFORMAT_INDEX1LSB,
            index1_msb = SDL_PIXELFORMAT_INDEX1MSB,

            index2_lsb = SDL_PIXELFORMAT_INDEX2LSB,
            index2_msb = SDL_PIXELFORMAT_INDEX2MSB,

            index4_lsb = SDL_PIXELFORMAT_INDEX4LSB,
            index4_msb = SDL_PIXELFORMAT_INDEX4MSB,

            index8 = SDL_PIXELFORMAT_INDEX8,

            rgb332 = SDL_PIXELFORMAT_RGB332,
            rgb565 = SDL_PIXELFORMAT_RGB565,

            bgr565 = SDL_PIXELFORMAT_BGR565,

            rgb24 = SDL_PIXELFORMAT_RGB24,
            bgr24 = SDL_PIXELFORMAT_BGR24,

            rgba4444 = SDL_PIXELFORMAT_RGBA4444,
            rgba5551 = SDL_PIXELFORMAT_RGBA5551,
            rgba8888 = SDL_PIXELFORMAT_RGBA8888,

            rgbx8888 = SDL_PIXELFORMAT_RGBX8888,

            xrgb4444 = SDL_PIXELFORMAT_XRGB4444,
            xrgb1555 = SDL_PIXELFORMAT_XRGB1555,
            xrgb8888 = SDL_PIXELFORMAT_XRGB8888,

            argb4444 = SDL_PIXELFORMAT_ARGB4444,
            argb1555 = SDL_PIXELFORMAT_ARGB1555,
            argb8888 = SDL_PIXELFORMAT_ARGB8888,

            abgr4444 = SDL_PIXELFORMAT_ABGR4444,
            abgr1555 = SDL_PIXELFORMAT_ABGR1555,
            abgr8888 = SDL_PIXELFORMAT_ABGR8888,

            bgra4444 = SDL_PIXELFORMAT_BGRA4444,
            bgra5551 = SDL_PIXELFORMAT_BGRA5551,
            bgra8888 = SDL_PIXELFORMAT_BGRA8888,

            bgrx8888 = SDL_PIXELFORMAT_BGRX8888,

            xbgr4444 = SDL_PIXELFORMAT_XBGR4444,
            xbgr1555 = SDL_PIXELFORMAT_XBGR1555,
            xbgr8888 = SDL_PIXELFORMAT_XBGR8888,

            argb2101010 = SDL_PIXELFORMAT_ARGB2101010,

            // Endianness-dependent aliases.
            // As such, hal::to_string'ing them will return the actual format it's referencing.
            rgba32 = SDL_PIXELFORMAT_RGBA32,
            argb32 = SDL_PIXELFORMAT_ARGB32,
            bgra32 = SDL_PIXELFORMAT_BGRA32,
            abgr32 = SDL_PIXELFORMAT_ABGR32,
            rgbx32 = SDL_PIXELFORMAT_RGBX32,
            xrgb32 = SDL_PIXELFORMAT_XRGB32,
            bgrx32 = SDL_PIXELFORMAT_BGRX32,
            xbgr32 = SDL_PIXELFORMAT_XBGR32,

            yv12 = SDL_PIXELFORMAT_YV12,
            iyuv = SDL_PIXELFORMAT_IYUV,
            yuy2 = SDL_PIXELFORMAT_YUY2,
            uyvy = SDL_PIXELFORMAT_UYVY,
            yvyu = SDL_PIXELFORMAT_YVYU,
            nv12 = SDL_PIXELFORMAT_NV12,
            nv21 = SDL_PIXELFORMAT_NV21
        };

        // Ways in which pixels are stored.
        enum class storage
        {
            unknown = SDL_PIXELTYPE_UNKNOWN,

            index1 = SDL_PIXELTYPE_INDEX1,
            index2 = SDL_PIXELTYPE_INDEX2,
            index4 = SDL_PIXELTYPE_INDEX4,
            index8 = SDL_PIXELTYPE_INDEX8,

            packed8  = SDL_PIXELTYPE_PACKED8,
            packed16 = SDL_PIXELTYPE_PACKED16,
            packed32 = SDL_PIXELTYPE_PACKED32,

            array_u8  = SDL_PIXELTYPE_ARRAYU8,
            array_u16 = SDL_PIXELTYPE_ARRAYU16,
            array_u32 = SDL_PIXELTYPE_ARRAYU32,
            array_f16 = SDL_PIXELTYPE_ARRAYF16,
            array_f32 = SDL_PIXELTYPE_ARRAYF32
        };

        constexpr storage storage_of(format fmt)
        {
            return static_cast<storage>(SDL_PIXELTYPE(static_cast<SDL_PixelFormatEnum>(fmt)));
        }
    }

    constexpr std::string_view to_string(pixel::format fmt)
    {
        using enum pixel::format;

        switch (fmt)
        {
        case unknown:
            return "Unknown";

        case index1_lsb:
            return "Index 1 LSB";

        case index1_msb:
            return "Index 1 MSB";

        case index2_lsb:
            return "Index 2 LSB";

        case index2_msb:
            return "Index 2 MSB";

        case index4_lsb:
            return "Index 4 LSB";

        case index4_msb:
            return "Index 4 MSB";

        case index8:
            return "Index 8";

        case rgb332:
            return "RGB332";

        case rgb565:
            return "RGB565";

        case bgr565:
            return "BGR565";

        case rgb24:
            return "RGB24";

        case bgr24:
            return "BGR24";

        case rgba4444:
            return "RGBA4444";

        case rgba5551:
            return "RGBA5551";

        case rgba8888:
            return "RGBA8888";

        case rgbx8888:
            return "RGBX8888";

        case xrgb4444:
            return "XRGB4444";

        case xrgb1555:
            return "XRGB1555";

        case xrgb8888:
            return "XRGB8888";

        case argb4444:
            return "ARGB4444";

        case argb1555:
            return "ARGB1555";

        case argb8888:
            return "ARGB8888";

        case abgr4444:
            return "ABGR4444";

        case abgr1555:
            return "ABGR1555";

        case abgr8888:
            return "ABGR8888";

        case bgra4444:
            return "BGRA4444";

        case bgra5551:
            return "BGRA5551";

        case bgra8888:
            return "BGRA8888";

        case bgrx8888:
            return "BGRX8888";

        case xbgr4444:
            return "XBGR4444";

        case xbgr1555:
            return "XBGR1555";

        case xbgr8888:
            return "XBGR8888";

        case argb2101010:
            return "ARGB2101010";

        case yv12:
            return "YV12";

        case iyuv:
            return "IYUV";

        case yuy2:
            return "YUY2";

        case uyvy:
            return "UYVY";

        case yvyu:
            return "YVYU";

        case nv12:
            return "NV12";

        case nv21:
            return "NV21";

        default:
            return "[unknown]";
        }
    }

    constexpr std::string_view to_string(pixel::storage tp)
    {
        using enum pixel::storage;

        switch (tp)
        {
        case unknown:
            return "Unknown";

        case index1:
            return "Index 1";

        case index2:
            return "Index 2";

        case index4:
            return "Index 4";

        case index8:
            return "Index 8";

        case packed8:
            return "Packed 8";

        case packed16:
            return "Packed 16";

        case packed32:
            return "Packed 32";

        case array_u8:
            return "Array u8";

        case array_u16:
            return "Array u16";

        case array_u32:
            return "Array u32";

        case array_f16:
            return "Array f16";

        case array_f32:
            return "Array f32";

        default:
            return "[unknown]";
        }
    }

    namespace coord
    {
        using point = point<coord_t>;
        using rect  = rectangle<coord_t>;
    }
}