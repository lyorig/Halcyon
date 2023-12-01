#include "texture.hpp"

#include "debug.hpp"
#include "halcyon/internal/SDL_types.hpp"
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
        SDL_TEXTUREACCESS_TARGET, size.x, size.y) }
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

lyo::u8 texture::opacity() const
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT(::SDL_GetTextureAlphaMod(m_object.get(), &alpha) == 0,
        ::SDL_GetError());

    return lyo::u8(alpha);
}

void texture::set_as_target() { window.renderer.set_target(*this); }

pixel_size texture::vw(lyo::f64 percent) const
{
    const pixel_type width { pixel_type(window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64 scale { width / static_cast<lyo::f64>(m_size.x) };

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
    this->reassign(
        ::SDL_CreateTextureFromSurface(window.renderer.ptr(), image.ptr()));

    m_size = image.size();

    return *this;
}

pixel_size texture::internal_size() const
{
    int w, h;

    ::SDL_QueryTexture(m_object.get(), nullptr, nullptr, &w, &h);

    return { pixel_type(w), pixel_type(h) };
}

// Drawer code.
using d = texture::drawer;

constexpr SDL_pixel_type no_size { std::numeric_limits<decltype(no_size)>::max() };

d::drawer(const texture& src)
    : m_src { .x = no_size }
    , m_this { src }
{
}

d& d::to(const coordinate& pos)
{
    m_dst.pos = fpoint_wrap(pos);
    m_dst.size = fpoint_wrap(m_this.size());
    return *this;
}

d& d::to(const world_area& area)
{
    m_dst = frect_wrap(area);
    return *this;
}

d& d::from(const pixel_area& src)
{
    m_src = src;
    return *this;
}

d& d::scale(lyo::f64 mul)
{
    m_dst *= mul;
    return *this;
}

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
    HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_this.window.renderer.ptr(), m_this.ptr(), m_src.x == no_size ? nullptr : &m_src, m_dst.addr(), m_angle, nullptr, SDL_RendererFlip(m_flip)) == 0, ::SDL_GetError());
}