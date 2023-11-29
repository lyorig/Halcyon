#include "texture.hpp"

#include "debug.hpp"
#include "window.hpp"

using namespace hal;

texture::texture(const class window& wnd) noexcept
    : m_size { 0, 0 }
    , window { wnd }
{
}

texture::texture(const class window& wnd, const pixel_size& size) noexcept
    : sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(),
        ::SDL_GetWindowPixelFormat(wnd.ptr()),
        SDL_TEXTUREACCESS_TARGET, size.x, size.y) }
    , m_size { size }
    , window { wnd }
{
}

texture::texture(const class window& wnd, const surface& image) noexcept
    : sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(),
        image.ptr()) }
    , m_size { image.size() }
    , window { wnd }
{
}

const pixel_size& texture::size() const noexcept { return m_size; }

lyo::u8 texture::opacity() const noexcept
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT(::SDL_GetTextureAlphaMod(m_object.get(), &alpha) == 0,
        ::SDL_GetError());

    return static_cast<lyo::u8>(alpha);
}

void texture::set_as_target() noexcept { window.renderer.set_target(*this); }

pixel_size texture::vw(lyo::f64 percent) const noexcept
{
    const pixel_type width { static_cast<pixel_type>(window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64 scale { width / static_cast<lyo::f64>(m_size.x) };

    return { width, static_cast<pixel_type>(m_size.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const noexcept
{
    const pixel_type height {
        static_cast<pixel_type>(window.size().y * (percent / 100.0))
    };
    const lyo::f64 scale { height / static_cast<lyo::f64>(m_size.y) };

    return { static_cast<pixel_type>(m_size.x * scale), height };
}

texture& texture::operator=(const surface& image) noexcept
{
    this->reassign(
        ::SDL_CreateTextureFromSurface(window.renderer.ptr(), image.ptr()));

    m_size = image.size();

    return *this;
}

pixel_size texture::internal_size() const noexcept
{
    int w, h;

    ::SDL_QueryTexture(m_object.get(), NULL, NULL, &w, &h);

    return { static_cast<pixel_type>(w), static_cast<pixel_type>(h) };
}

// Drawer code.
using d = texture::drawer;

d::drawer(const texture& src) noexcept
    : m_dst { as_size, static_cast<coordinate>(src.size()) }
    , m_this { src }
{
}

d& d::to(const coordinate& pos) noexcept
{
    m_dst.pos = pos;
    return *this;
}

d& d::to(const world_area& area) noexcept
{
    m_dst = area;
    return *this;
}

d& d::from(const pixel_area& src) noexcept
{
    m_src = src;
    return *this;
}

d& d::scale(lyo::f64 mul) noexcept
{
    m_scale = mul;
    return *this;
}

d& d::rotate(lyo::f64 angle) noexcept
{
    m_angle = angle;
    return *this;
}

void d::operator()() const noexcept
{
}