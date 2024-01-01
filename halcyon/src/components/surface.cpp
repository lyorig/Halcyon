#include <halcyon/components/surface.hpp>

using namespace hal;

void blit::operator()(const surface& dst)
{
    constexpr st us { unset<st> };

    m_this.internal_blit(dst,
        m_src.pos.x == us ? nullptr : m_src.addr(),
        m_dst.pos.x == us ? nullptr : m_dst.addr(),
        {});
}

void blit::operator()(const surface& dst, keep_dest_tag) const
{
    constexpr st us { unset<st> };

    SDL::Rect copy { m_dst };

    m_this.internal_blit(dst,
        m_src.pos.x == us ? nullptr : m_src.addr(),
        copy.pos.x == us ? nullptr : copy.addr(),
        {});
}

surface::surface(const video& sys, pixel_size sz)
    : surface { sz }
{
}

surface::surface(pixel_size sz)
    : surface { ::SDL_CreateRGBSurface(0, sz.x, sz.y, 32, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF) }
{
}

surface::surface(SDL_Surface* surf)
    : sdl_object { surf }
{
}

surface surface::resize(pixel_size sz)
{
    surface    ret { sz };
    blend_lock bl { *this, blend_mode::none };

    hal::blit { *this }.to(hal::fill)(ret);

    return ret;
}

surface surface::resize(lyo::f64 scale)
{
    return this->resize(this->size() * scale);
}

pixel_size surface::size() const
{
    return { static_cast<pixel_type>(this->ptr()->w),
        static_cast<pixel_type>(this->ptr()->h) };
}

color surface::operator[](pixel_pos coord) const
{
    HAL_DEBUG_ASSERT(coord.x < ptr()->w, "Out-of-range width");
    HAL_DEBUG_ASSERT(coord.y < ptr()->h, "Out-of-range height");

    color ret;

    // I couldn't find any mention of this function having a fail state.
    ::SDL_GetRGBA(this->get_pixel(coord.x, coord.y), ptr()->format, &ret.r,
        &ret.g, &ret.b, &ret.a);

    return ret;
}

void surface::internal_blit(const surface& to, const SDL_Rect* src, SDL_Rect* dst, lyo::pass_key<blit>) const
{
    HAL_DEBUG_ASSERT(this->ptr() != nullptr, "Drawing null surface");
    HAL_DEBUG_ASSERT(to.ptr() != nullptr, "Drawing to null surface");

    ::SDL_BlitSurface(this->ptr(), src, to.ptr(), dst);
}

blend_mode surface::get_blend() const
{
    SDL_BlendMode bm;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetSurfaceBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void surface::set_blend(blend_mode bm)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetSurfaceBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

Uint32 surface::get_pixel(pixel_type x, pixel_type y) const
{
    const auto   bpp { ptr()->format->BytesPerPixel };
    const Uint8* p { static_cast<Uint8*>(ptr()->pixels) + y * ptr()->pitch + x * bpp };

    switch (bpp)
    {
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
        HAL_DEBUG_PANIC("Unknown bytes-per-pixel value while getting pixel from surface");

        return 0;
    }
}