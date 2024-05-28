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

    // Formats in which pixels are stored.
    // Warning: not exhaustive.
    enum class pixel_format : u32
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

    constexpr std::string_view to_string(pixel_format fmt)
    {
        using enum pixel_format;

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

    using pixel_point = point<pixel_t>;
    using pixel_rect  = rectangle<pixel_t>;

    using coord_point = point<coord_t>;
    using coord_rect  = rectangle<coord_t>;

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