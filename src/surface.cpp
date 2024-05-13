#include <halcyon/surface.hpp>

#include <climits>
#include <cstring>

#include <SDL_image.h>

using namespace hal;

// Set the depth accordingly upon changing this value.
constexpr SDL_PixelFormatEnum default_format { SDL_PIXELFORMAT_RGBA32 };

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
    : raii_object { ::SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, CHAR_BIT * 4, default_format) }
{
}

surface::surface(accessor src)
    : raii_object { ::SDL_LoadBMP_RW(src.use(pass_key<surface> {}), true) }
{
}

surface::surface(SDL_Surface* ptr, pass_key<image::context>)
    : raii_object { ptr }
{
}

surface::surface(SDL_Surface* ptr, pass_key<font>)
    : raii_object { ptr }
{
}

void surface::fill(color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(get(), nullptr, mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rect(const sdl::pixel_rect& area, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(get(), area.addr(), mapped(clr)) == 0, debug::last_error());
}

void surface::fill_rects(const std::span<const sdl::pixel_rect>& areas, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRects(get(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), mapped(clr)) == 0, debug::last_error());
}

surface surface::resize(pixel_point sz)
{
    surface    ret { sz };
    blend_lock bl { *this, blend_mode::none };

    if (get()->format->format == default_format)
        blit(ret).to(tag::fill)();

    else
        surface { *this, default_format }.blit(ret).to(tag::fill)();

    return ret;
}

surface surface::resize(scaler scl)
{
    return resize(scl(size()));
}

void surface::save(outputter dst) const
{
    HAL_ASSERT_VITAL(::SDL_SaveBMP_RW(get(), dst.use(pass_key<surface> {}), true) == 0, debug::last_error());
}

blitter surface::blit(surface& dst) const
{
    return { *this, dst, pass_key<surface> {} };
}

pixel_point surface::size() const
{
    return {
        pixel_t(get()->w),
        pixel_t(get()->h)
    };
}

blend_mode surface::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetSurfaceBlendMode(get(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void surface::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetSurfaceBlendMode(get(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

pixel_reference surface::operator[](const pixel_point& pos) const
{
    HAL_ASSERT(pos.x < get()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < get()->h, "Out-of-range height");

    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
}

surface::surface(const surface& cvt, SDL_PixelFormatEnum fmt)
    : raii_object { ::SDL_ConvertSurfaceFormat(cvt.get(), fmt, 0) }
{
}

std::uint32_t surface::mapped(color c) const
{
    return ::SDL_MapRGBA(get()->format, c.r, c.g, c.b, c.a);
}

pixel_reference::pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel_point pos, pass_key<surface>)
    : m_ptr { pixels + pos.y * pitch + pos.x * fmt->BytesPerPixel }
    , m_fmt { fmt }
{
}

color pixel_reference::color() const
{
    hal::color c;
    ::SDL_GetRGBA(get_mapped(), m_fmt, &c.r, &c.g, &c.b, &c.a);
    return c;
}

void pixel_reference::color(struct color c)
{
    set_mapped(::SDL_MapRGBA(m_fmt, c.r, c.g, c.b, c.a));
}

std::uint32_t pixel_reference::get_mapped() const
{
    std::uint32_t ret { 0 };

    if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        std::memcpy(&ret, m_ptr, m_fmt->BytesPerPixel);

    else
    {
        const u8 offset = sizeof(std::uint32_t) - m_fmt->BytesPerPixel;
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->BytesPerPixel);
    }

    return ret;
}

void pixel_reference::set_mapped(std::uint32_t mapped)
{
    if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        std::memcpy(m_ptr, &mapped, m_fmt->BytesPerPixel);

    else
    {
        const u8 offset = sizeof(std::uint32_t) - m_fmt->BytesPerPixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->BytesPerPixel);
    }
}

void blitter::operator()()
{
    HAL_ASSERT_VITAL(::SDL_BlitScaled(
                         m_pass.get(),
                         m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                         m_this.get(),
                         m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<SDL_Rect*>(m_dst.addr()))
            == 0,
        debug::last_error());
}

void blitter::operator()(HAL_TAG_NAME(keep_dst)) const
{
    sdl::pixel_rect copy { m_dst };

    HAL_ASSERT_VITAL(::SDL_BlitScaled(
                         m_pass.get(),
                         reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                         m_this.get(),
                         reinterpret_cast<SDL_Rect*>(copy.addr()))
            == 0,
        debug::last_error());
}