#include "texture.hpp"

#include <halcyon/debug.hpp>
#include <halcyon/window.hpp>

using namespace hal;

texture::texture(const class window& wnd)
    : window { wnd }
{
}

texture::texture(const class window& wnd, const pixel_size& size)
    : sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(),
        ::SDL_GetWindowPixelFormat(wnd.ptr()),
        SDL_TEXTUREACCESS_STATIC, size.x, size.y) }
    , window { wnd }
{
}

texture::texture(const class window& wnd, const surface& image)
    : sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(),
        image.ptr()) }
    , window { wnd }
{
}

pixel_size texture::size() const
{
    return this->internal_size();
}

void texture::set_opacity(color::value_type value)
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

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetTextureAlphaMod(this->ptr(), &alpha) == 0,
        ::SDL_GetError());

    return lyo::u8(alpha);
}

pixel_size texture::vw(lyo::f64 percent) const
{
    const pixel_size sz { this->size() };
    const pixel_type width { pixel_type(window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64   scale { width / static_cast<lyo::f64>(sz.x) };

    return { width, pixel_type(sz.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const
{
    const pixel_size sz { this->size() };
    const pixel_type height {
        pixel_type(window.size().y * (percent / 100.0))
    };
    const lyo::f64 scale { height / static_cast<lyo::f64>(sz.y) };

    return { pixel_type(sz.x * scale), height };
}

texture& texture::operator=(const surface& image)
{
    sdl_object::operator=(::SDL_CreateTextureFromSurface(window.renderer.ptr(), image.ptr()));
    return *this;
}

pixel_size texture::internal_size() const
{
    int w, h;

    ::SDL_QueryTexture(this->ptr(), nullptr, nullptr, &w, &h);

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