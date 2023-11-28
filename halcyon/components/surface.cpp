#include "surface.hpp"

#include "SDL2/SDL_surface.h"

using namespace hal;

surface::surface(window& wnd, pixel_size sz) noexcept
    : surface { sz }
{
}

surface::surface(pixel_size sz) noexcept
    : surface { ::SDL_CreateRGBSurface(0, sz.x, sz.y, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF) }
{
}

surface::surface(SDL_Surface* surf) noexcept
    : sdl_object { surf }
{
}

surface surface::resize(pixel_size sz) const noexcept
{
    surface ret { sz };

    this->set_blend(SDL_BLENDMODE_NONE);

    HAL_DEBUG_ASSERT(
        ::SDL_BlitScaled(this->ptr(), nullptr, ret.ptr(), nullptr) == 0,
        ::SDL_GetError());

    this->set_blend(SDL_BLENDMODE_BLEND);

    return ret;
}

surface surface::resize(lyo::f64 scale) const noexcept
{
    return this->resize(this->size() * scale);
}

pixel_size surface::size() const noexcept
{
    return { static_cast<pixel_type>(m_object->w),
        static_cast<pixel_type>(m_object->h) };
}

color surface::operator[](pixel_pos coord) const noexcept
{
    HAL_DEBUG_CHECK(coord.x < ptr()->w, "Out-of-range width");
    HAL_DEBUG_CHECK(coord.y < ptr()->h, "Out-of-range height");

    color ret;

    // I couldn't find any mention of this function having a fail state.
    ::SDL_GetRGBA(this->get_pixel(coord.x, coord.y), ptr()->format, &ret.r,
        &ret.g, &ret.b, &ret.a);

    return ret;
}

void surface::set_blend(SDL_BlendMode bm) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetSurfaceBlendMode(this->ptr(), bm) == 0, ::SDL_GetError());
}

Uint32 surface::get_pixel(pixel_type x, pixel_type y) const noexcept
{
    const auto bpp { ptr()->format->BytesPerPixel };
    const Uint8* p { static_cast<Uint8*>(ptr()->pixels) + y * ptr()->pitch + x * bpp };

    switch (bpp) {
    case 1:
        return *p;

    case 2:
        return *reinterpret_cast<const Uint16*>(p);

    case 3:
        if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];

        else
            return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
        return *reinterpret_cast<const Uint32*>(p);

    default: // Intentionally panic.
        HAL_DEBUG_CHECK(
            false,
            "Unknown bytes-per-pixel value while getting pixel from surface");

        return 0;
    }
}