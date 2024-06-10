#include <halcyon/surface.hpp>

#include <climits>
#include <cstring>

#include <SDL_image.h>

#include <halcyon/utility/locks.hpp>

using namespace hal;

surface::surface(pixel::point sz, pixel::format fmt)
    : raii_object { ::SDL_CreateRGBSurfaceWithFormat(0, sz.x, sz.y, CHAR_BIT * 4, static_cast<Uint32>(fmt)) }
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

surface::surface(SDL_Surface* ptr, pass_key<builder::font_text>)
    : raii_object { ptr }
{
}

surface::surface(SDL_Surface* ptr, pass_key<builder::font_glyph>)
    : raii_object { ptr }
{
}

void surface::fill(color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(get(), nullptr, mapped(clr)) == 0, debug::last_error());
}

void surface::fill(pixel::rect area, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRect(get(), area.addr(), mapped(clr)) == 0, debug::last_error());
}

void surface::fill(std::span<const pixel::rect> areas, color clr)
{
    HAL_ASSERT_VITAL(::SDL_FillRects(get(), reinterpret_cast<const SDL_Rect*>(areas.data()), static_cast<int>(areas.size()), mapped(clr)) == 0, debug::last_error());
}

surface surface::resize(pixel::point sz)
{
    surface     ret { sz };
    lock::blend bl { *this, blend_mode::none };

    blit(ret).to(tag::fill)();

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
    return { dst, *this, pass_key<surface> {} };
}

surface surface::convert(pixel::format fmt) const
{
    return { *this, fmt };
}

pixel::point surface::size() const
{
    return {
        pixel_t(get()->w),
        pixel_t(get()->h)
    };
}

pixel::format surface::pixel_format() const
{
    return static_cast<pixel::format>(get()->format->format);
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

color surface::color_mod() const
{
    color ret;

    HAL_ASSERT_VITAL(::SDL_GetSurfaceColorMod(get(), &ret.r, &ret.g, &ret.b) == 0, debug::last_error());

    return ret;
}

void surface::color_mod(color col)
{
    HAL_ASSERT_VITAL(::SDL_SetSurfaceColorMod(get(), col.r, col.g, col.b) == 0, debug::last_error());
}

color::value_t surface::alpha_mod() const
{
    color::value_t ret;

    HAL_ASSERT_VITAL(::SDL_GetSurfaceAlphaMod(get(), &ret) == 0, debug::last_error());

    return ret;
}

void surface::alpha_mod(color::value_t val)
{
    HAL_ASSERT_VITAL(::SDL_SetSurfaceAlphaMod(get(), val) == 0, debug::last_error());
}

pixel_reference surface::operator[](const pixel::point& pos) const
{
    HAL_ASSERT(pos.x < get()->w, "Out-of-range width");
    HAL_ASSERT(pos.y < get()->h, "Out-of-range height");

    return { static_cast<std::byte*>(get()->pixels), get()->pitch, get()->format, pos, pass_key<surface> {} };
}

surface::surface(const surface& cvt, pixel::format fmt)
    : raii_object { ::SDL_ConvertSurfaceFormat(cvt.get(), static_cast<Uint32>(fmt), 0) }
{
}

std::uint32_t surface::mapped(color c) const
{
    return ::SDL_MapRGBA(get()->format, c.r, c.g, c.b, c.a);
}

pixel_reference::pixel_reference(std::byte* pixels, int pitch, const SDL_PixelFormat* fmt, pixel::point pos, pass_key<surface>)
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

Uint32 pixel_reference::get_mapped() const
{
    Uint32 ret { 0 };

    if constexpr (SDL_BYTEORDER == SDL_LIL_ENDIAN)
        std::memcpy(&ret, m_ptr, m_fmt->BytesPerPixel);

    else
    {
        const u8 offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(&ret + offset, m_ptr + offset, m_fmt->BytesPerPixel);
    }

    return ret;
}

void pixel_reference::set_mapped(Uint32 mapped)
{
    if constexpr (compile_settings::byte_order == byte_order::lil_endian)
        std::memcpy(m_ptr, &mapped, m_fmt->BytesPerPixel);

    else
    {
        const u8 offset = sizeof(Uint32) - m_fmt->BytesPerPixel;
        std::memcpy(m_ptr + offset, &mapped + offset, m_fmt->BytesPerPixel);
    }
}

void blitter::operator()()
{
    HAL_ASSERT_VITAL(::SDL_BlitScaled(
                         m_this.get(),
                         m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                         m_pass.get(),
                         m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<SDL_Rect*>(m_dst.addr()))
            == 0,
        debug::last_error());
}

void blitter::operator()(HAL_TAG_NAME(keep_dst)) const
{
    pixel::rect copy { m_dst };

    HAL_ASSERT_VITAL(::SDL_BlitScaled(
                         m_this.get(),
                         m_src.pos.x == unset_pos<src_t>() ? nullptr : m_src.addr(),
                         m_pass.get(),
                         m_dst.pos.x == unset_pos<dst_t>() ? nullptr : copy.addr())
            == 0,
        debug::last_error());
}