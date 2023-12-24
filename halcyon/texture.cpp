#include "texture.hpp"

#include "debug.hpp"
#include "halcyon/internal/SDL_types.hpp"
#include "halcyon/types/point.hpp"
#include "window.hpp"

using namespace hal;

texture::texture(const class window& wnd)
    : m_size { 0, 0 }
    , window { wnd }
{
}

texture::texture(const class window& wnd, const pixel_size& size)
    : sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(),
        ::SDL_GetWindowPixelFormat(wnd.ptr()),
        SDL_TEXTUREACCESS_STATIC, size.x, size.y) }
    , m_size { size }
    , window { wnd }
{
}

texture::texture(const class window& wnd, const surface& image)
    : sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(),
        image.ptr()) }
    , m_size { image.size() }
    , window { wnd }
{
}

const pixel_size& texture::size() const { return m_size; }

void texture::set_opacity(lyo::u8 value)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetTextureAlphaMod(this->ptr(), value) == 0, ::SDL_GetError());
}

void texture::set_color_mod(color clr)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetTextureColorMod(this->ptr(), clr.r, clr.g, clr.b) == 0, ::SDL_GetError());
}

lyo::u8 texture::opacity() const
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetTextureAlphaMod(m_object.get(), &alpha) == 0,
        ::SDL_GetError());

    return lyo::u8(alpha);
}

pixel_size texture::vw(lyo::f64 percent) const
{
    const pixel_type width { pixel_type(window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64   scale { width / static_cast<lyo::f64>(m_size.x) };

    return { width, pixel_type(m_size.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const
{
    const pixel_type height {
        pixel_type(window.size().y * (percent / 100.0))
    };
    const lyo::f64 scale { height / static_cast<lyo::f64>(m_size.y) };

    return { pixel_type(m_size.x * scale), height };
}

texture& texture::operator=(const surface& image)
{
    sdl_object::operator=(::SDL_CreateTextureFromSurface(window.renderer.ptr(), image.ptr()));

    m_size = image.size();

    return *this;
}

pixel_size texture::internal_size() const
{
    int w, h;

    ::SDL_QueryTexture(m_object.get(), nullptr, nullptr, &w, &h);

    return { pixel_type(w), pixel_type(h) };
}

using d = texture::draw;

d& d::rotate(lyo::f64 angle)
{
    m_angle = angle;
    return *this;
}

d& d::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

void d::operator()() const
{
    if (m_this.ptr() != nullptr)
        HAL_DEBUG_ASSERT_VITAL(::SDL_RenderCopyExF(m_this.window.renderer.ptr(), m_this.ptr(), m_src.pos.x == unset<st> ? nullptr : m_src.addr(), m_dst.addr(), m_angle, nullptr, SDL_RendererFlip(m_flip))
                == 0,
            ::SDL_GetError());
}