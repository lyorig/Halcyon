#include <halcyon/surface.hpp>

using namespace hal;

// Set the depth accordingly upon changing this value.
constexpr SDL_PixelFormatEnum default_format { SDL_PIXELFORMAT_RGBA32 };

surface::surface(pixel_point sz)
    : surface { sz, CHAR_BIT * 4, default_format }
{
}

surface::surface(SDL_Surface* surf)
    : object { surf }
{
}

surface::surface(pixel_point sz, int depth, Uint32 fmt)
    : object { ::SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, depth, fmt) }
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
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), nullptr, mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rect(const sdl::pixel_rect& area, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(ptr(), area.addr(), mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRects(ptr(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), mapped(clr)) == 0, debug::last_error());
}

pixel_point surface::size() const
{
    return {
        pixel_t(this->ptr()->w),
        pixel_t(this->ptr()->h)
    };
}

pixel_reference surface::operator[](const pixel_point& pos) const
{
    HAL_ASSERT(pos.x < ptr()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < ptr()->h, "Out-of-range height");

    return { ptr()->pixels, ptr()->pitch, ptr()->format, pos };
}

blend_mode surface::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetSurfaceBlendMode(this->ptr(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void surface::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetSurfaceBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

blitter surface::blit(surface& dst) const
{
    return { *this, dst, {} };
}

void surface::internal_blit(const surface& to, const sdl::pixel_rect* src, sdl::pixel_rect* dst, lyo::pass_key<blitter>) const
{
    HAL_ASSERT(this->exists(), "Drawing null surface");
    HAL_ASSERT(to.exists(), "Drawing to null surface");

    HAL_ASSERT_VITAL(::SDL_BlitScaled(this->ptr(), reinterpret_cast<const SDL_Rect*>(src), to.ptr(), reinterpret_cast<SDL_Rect*>(dst)) == 0, debug::last_error());
}

Uint32 surface::mapped(color c) const
{
    return ::SDL_MapRGBA(ptr()->format, c.r, c.g, c.b, c.a);
}

pixel_reference::pixel_reference(void* pixels, int pitch, const SDL_PixelFormat* fmt, pixel_point pos)
    : m_ptr { static_cast<std::byte*>(pixels) + pos.y * pitch + pos.x * fmt->BytesPerPixel }
    , m_fmt { fmt }
{
}

pixel_reference::operator color() const
{
    color c;
    ::SDL_GetRGBA(get(), m_fmt, &c.r, &c.g, &c.b, &c.a);
    return c;
}

pixel_reference& pixel_reference::operator=(color c)
{
    set(::SDL_MapRGBA(m_fmt, c.r, c.g, c.b, c.a));
    return *this;
}

Uint32 pixel_reference::get() const
{
    Uint32 ret { 0 };

    if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        std::memcpy(&ret, m_ptr, m_fmt->BytesPerPixel);

    else
    {
        const lyo::u8 offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->BytesPerPixel);
    }

    return ret;
}

void pixel_reference::set(Uint32 mapped)
{
    if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        std::memcpy(m_ptr, &mapped, m_fmt->BytesPerPixel);

    else
    {
        const lyo::u8 offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->BytesPerPixel);
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
    sdl::pixel_rect copy { m_dst };

    m_pass.internal_blit(
        m_this,
        m_src.pos.x == unset<src_t> ? nullptr : &m_src,
        copy.pos.x == unset<dst_t> ? nullptr : &copy,
        {});
}