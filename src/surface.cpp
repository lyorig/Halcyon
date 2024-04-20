#include <halcyon/surface.hpp>

#include <climits>
#include <cstring>

using namespace hal;

// Set the depth accordingly upon changing this value.
constexpr SDL_PixelFormatEnum default_format { SDL_PIXELFORMAT_RGBA32 };
constexpr SDL_ScaleMode       default_scale_mode { SDL_SCALEMODE_LINEAR };

surface::blend_lock::blend_lock(surface& surf, blend_mode bm)
    : m_surf { surf }
    , m_old { surf.blend() }
{
    m_surf.blend(bm);
}

surface::blend_lock::~blend_lock()
{
    m_surf.blend(m_old);
}

void surface::blend_lock::set(blend_mode bm)
{
    m_surf.blend(bm);
}

surface::surface(pixel_point sz)
    : surface { ::SDL_CreateSurface(sz.x, sz.y, default_format) }
{
}

surface surface::resize(pixel_point sz)
{
    surface    ret { sz };
    blend_lock bl { *this, blend_mode::none };

    if (ptr()->format->format == default_format)
        blit(ret).to(tag::fill)();

    else
        convert().blit(ret).to(tag::fill)();

    return ret;
}

surface surface::resize(f64 scale)
{
    return this->resize(this->size() * scale);
}

void surface::fill(color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillSurfaceRect(ptr(), nullptr, mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rect(const sdl::pixel_rect& area, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillSurfaceRect(ptr(), area.addr(), mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillSurfaceRects(ptr(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), mapped(clr)) == 0, debug::last_error());
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

surface::surface(SDL_Surface* ptr)
    : object { ptr }
{
}

surface surface::convert() const
{
    return ::SDL_ConvertSurfaceFormat(ptr(), default_format);
}

std::uint32_t surface::mapped(color c) const
{
    return ::SDL_MapRGBA(ptr()->format, c.r, c.g, c.b, c.a);
}

pixel_reference::pixel_reference(void* pixels, int pitch, const SDL_PixelFormat* fmt, pixel_point pos)
    : m_ptr { static_cast<std::byte*>(pixels) + pos.y * pitch + pos.x * fmt->bytes_per_pixel }
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

std::uint32_t pixel_reference::get() const
{
    std::uint32_t ret { 0 };

    if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        std::memcpy(&ret, m_ptr, m_fmt->bytes_per_pixel);

    else
    {
        const u8 offset = sizeof(std::uint32_t) - m_fmt->bytes_per_pixel;
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->bytes_per_pixel);
    }

    return ret;
}

void pixel_reference::set(std::uint32_t mapped)
{
    if constexpr (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        std::memcpy(m_ptr, &mapped, m_fmt->bytes_per_pixel);

    else
    {
        const u8 offset = sizeof(std::uint32_t) - m_fmt->bytes_per_pixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->bytes_per_pixel);
    }
}

void blitter::operator()()
{
    HAL_ASSERT_VITAL(::SDL_BlitSurfaceScaled(
                         m_pass.ptr(),
                         m_src.pos.x == detail::unset_pos<render_t> ? nullptr : m_src.addr(),
                         m_this.ptr(),
                         m_dst.pos.x == detail::unset_pos<render_t> ? nullptr : m_dst.addr(),
                         default_scale_mode)
            == 0,
        debug::last_error());
}

void blitter::operator()(HAL_TAG_NAME(keep_dst)) const
{
    sdl::pixel_rect copy { m_dst };

    HAL_ASSERT_VITAL(::SDL_BlitSurfaceScaled(
                         m_pass.ptr(),
                         m_src.addr(),
                         m_this.ptr(),
                         copy.addr(),
                         default_scale_mode)
            == 0,
        debug::last_error());
}