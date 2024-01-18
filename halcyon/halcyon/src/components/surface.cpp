#include <halcyon/components/surface.hpp>

using namespace hal;

// Set the depth accordingly upon changing this value.
constexpr SDL_PixelFormatEnum default_format { SDL_PIXELFORMAT_RGBA32 };

surface::surface(SDL_Surface* surf, lyo::pass_key<image_loader>)
    : object { surf }
{
}

surface::surface(SDL_Surface* surf, lyo::pass_key<font>)
    : object { surf }
{
}

surface::surface(pixel_point sz, int depth, Uint32 fmt)
    : object { ::SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, depth, fmt) }
{
}

surface::surface([[maybe_unused]] video& sys, pixel_point sz)
    : surface { sz, CHAR_BIT * 4, default_format }
{
}

surface::surface(pixel_point sz, const SDL_PixelFormat* fmt)
    : surface { sz, fmt->BitsPerPixel * 4, fmt->format }
{
}

surface surface::resize(pixel_point sz)
{
    surface    ret { sz, ptr()->format };
    blend_lock bl { *this, blend_mode::none };

    hal::blit { *this }.to(hal::fill)(ret);

    return ret;
}

surface surface::resize(lyo::f64 scale)
{
    return this->resize(this->size() * scale);
}

void surface::fill(color clr)
{
    const Uint32 mapped { ::SDL_MapRGBA(ptr()->format, clr.r, clr.g, clr.b, clr.a) };
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), nullptr, mapped) == 0, ::SDL_GetError());
}

void surface::fill_rect(const sdl::pixel_rect& area, color clr)
{
    const Uint32 mapped { ::SDL_MapRGBA(ptr()->format, clr.r, clr.g, clr.b, clr.a) };
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), area.addr(), mapped) == 0, ::SDL_GetError());
}

pixel_point surface::size() const
{
    return {
        pixel_t(this->ptr()->w),
        pixel_t(this->ptr()->h)
    };
}

color surface::operator[](const pixel_point& pos) const
{
    HAL_ASSERT(pos.x < ptr()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < ptr()->h, "Out-of-range height");

    color ret;

    // I couldn't find any mention of this function having a fail state.
    ::SDL_GetRGBA(this->pixel_at(pos), ptr()->format, &ret.r,
        &ret.g, &ret.b, &ret.a);

    return ret;
}

void surface::internal_blit(const surface& to, const SDL_Rect* src, SDL_Rect* dst, lyo::pass_key<blit>) const
{
    HAL_ASSERT(this->exists(), "Drawing null surface");
    HAL_ASSERT(to.exists(), "Drawing to null surface");

    HAL_ASSERT_VITAL(::SDL_BlitScaled(this->ptr(), src, to.ptr(), dst) == 0, ::SDL_GetError());
}

blend_mode surface::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetSurfaceBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void surface::set_blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetSurfaceBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

Uint32 surface::pixel_at(const pixel_point& pos) const
{
    const auto   bpp { ptr()->format->BytesPerPixel };
    const Uint8* p { static_cast<Uint8*>(ptr()->pixels) + pos.y * ptr()->pitch + pos.x * bpp };

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
        HAL_PANIC("Unknown bytes-per-pixel value while getting pixel from surface");

        return 0;
    }
}

void blit::operator()(const surface& dst)
{
    m_this.internal_blit(
        dst,
        m_src.pos.x == unset<src_t> ? nullptr : m_src.addr(),
        m_dst.pos.x == unset<dst_t> ? nullptr : m_dst.addr(),
        {});
}

void blit::operator()(const surface& dst, keep_dst_tag) const
{
    constexpr src_t us { unset<src_t> };

    sdl::pixel_rect copy { m_dst };

    m_this.internal_blit(
        dst,
        m_src.pos.x == us ? nullptr : m_src.addr(),
        copy.pos.x == us ? nullptr : copy.addr(),
        {});
}