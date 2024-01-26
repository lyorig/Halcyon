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

    this->blit(ret).to(hal::fill)();

    return ret;
}

surface surface::resize(lyo::f64 scale)
{
    return this->resize(this->size() * scale);
}

void surface::fill(color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), nullptr, mapped(clr)) == 0, ::SDL_GetError());
}

void surface::fill_rect(const sdl::pixel_rect& area, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), area.addr(), mapped(clr)) == 0, ::SDL_GetError());
}

void surface::fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRects(ptr(), reinterpret_cast<const SDL_Rect*>(areas.data()), areas.size(), mapped(clr)) == 0, ::SDL_GetError());
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

blitter surface::blit(surface& dst) const
{
    return { *this, dst, {} };
}

void surface::internal_blit(const surface& to, const sdl::pixel_rect* src, sdl::pixel_rect* dst, lyo::pass_key<blitter>) const
{
    HAL_ASSERT(this->exists(), "Drawing null surface");
    HAL_ASSERT(to.exists(), "Drawing to null surface");

    HAL_ASSERT_VITAL(::SDL_BlitScaled(this->ptr(), reinterpret_cast<const SDL_Rect*>(src), to.ptr(), reinterpret_cast<SDL_Rect*>(dst)) == 0, ::SDL_GetError());
}

Uint32 surface::mapped(color clr) const
{
    return ::SDL_MapRGBA(ptr()->format, clr.r, clr.g, clr.b, clr.a);
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

void blitter::operator()()
{
    m_pass.internal_blit(
        m_this,
        m_src.pos.x == unset<src_t> ? nullptr : &m_src,
        m_dst.pos.x == unset<dst_t> ? nullptr : &m_dst,
        {});
}

void blitter::operator()(keep_dst_tag) const
{
    constexpr src_t us { unset<src_t> };

    sdl::pixel_rect copy { m_dst };

    m_pass.internal_blit(
        m_this,
        m_src.pos.x == us ? nullptr : &m_src,
        copy.pos.x == us ? nullptr : &copy,
        {});
}