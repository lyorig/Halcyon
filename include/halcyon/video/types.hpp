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
        }
    }

    std::ostream& operator<<(std::ostream& str, blend_mode bm);

    namespace pixel
    {
        using point = point<pixel_t>;
        using rect  = rectangle<pixel_t>;

        // Formats in which pixels are stored.
        // Warning: not exhaustive.
        enum class format : u32
        {
            unknown = SDL_PIXELFORMAT_UNKNOWN,

            index8 = SDL_PIXELFORMAT_INDEX8,

            rgb24 = SDL_PIXELFORMAT_RGB24,
            bgr24 = SDL_PIXELFORMAT_BGR24,

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

        std::ostream& operator<<(std::ostream& str, pixel::format fmt);

        // Exhaustive list of all pixel representation types.
        enum class type
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

        std::ostream& operator<<(std::ostream& str, pixel::type tp);

        constexpr type type_of(format fmt)
        {
            return static_cast<type>(SDL_PIXELTYPE(static_cast<SDL_PixelFormatEnum>(fmt)));
        }
    }

    constexpr std::string_view to_string(pixel::format fmt)
    {
        using enum pixel::format;

        switch (fmt)
        {
        case unknown:
            return "Unknown";

        case index8:
            return "Index 8";

        case rgb24:
            return "RGB24";

        case bgr24:
            return "BGR24";

        case rgba32:
            return "RGBA32";

        case argb32:
            return "ARGB32";

        case bgra32:
            return "BGRA32";

        case abgr32:
            return "ABGR32";

        case rgbx32:
            return "RGBx32";

        case xrgb32:
            return "xRGB32";

        case bgrx32:
            return "BGRx32";

        case xbgr32:
            return "xBGR32";

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

    constexpr std::string_view to_string(pixel::type tp)
    {
        using enum pixel::type;

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

    namespace literals
    {
        constexpr pixel_t operator""_px(unsigned long long v)
        {
            return static_cast<pixel_t>(v);
        }

        constexpr pixel_t operator""_px(long double v)
        {
            return static_cast<pixel_t>(v);
        }

        constexpr coord_t operator""_crd(unsigned long long v)
        {
            return static_cast<coord_t>(v);
        }

        constexpr coord_t operator""_crd(long double v)
        {
            return static_cast<coord_t>(v);
        }
    }

    namespace sdl
    {
        template <meta::arithmetic T>
        using point_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Point, SDL_FPoint>;

        template <meta::arithmetic T>
        using rect_t = std::conditional_t<std::is_same_v<T, pixel_t>, SDL_Rect, SDL_FRect>;
    }

    // Paranoia.
    static_assert(std::is_signed_v<pixel_t>);
}