#include <halcyon/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/window.hpp>

using namespace hal;

texture::texture(class window& wnd)
    : m_window { wnd }
{
}

texture::texture(class window& wnd, const pixel_size& size, SDL_TextureAccess access)
    : sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(),
        ::SDL_GetWindowPixelFormat(wnd.ptr()),
        SDL_TEXTUREACCESS_STATIC, size.x, size.y) }
    , m_window { wnd }
{
}

texture::texture(class window& wnd, const surface& image, SDL_TextureAccess access)
    : sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(),
        image.ptr()) }
    , m_window { wnd }
{
}

pixel_size texture::size() const
{
    return this->internal_size();
}

void texture::set_opacity(color::value value)
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

const window& texture::window() const
{
    return m_window;
}

pixel_size texture::vw(lyo::f64 percent) const
{
    const pixel_size sz { this->size() };
    const pixel_type width { pixel_type(m_window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64   scale { width / static_cast<lyo::f64>(sz.x) };

    return { width, pixel_type(sz.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const
{
    const pixel_size sz { this->size() };
    const pixel_type height {
        pixel_type(m_window.size().y * (percent / 100.0))
    };
    const lyo::f64 scale { height / static_cast<lyo::f64>(sz.y) };

    return { pixel_type(sz.x * scale), height };
}

texture& texture::operator=(const surface& image)
{
    sdl_object::operator=(::SDL_CreateTextureFromSurface(m_window.renderer.ptr(), image.ptr()));
    return *this;
}

pixel_size texture::internal_size() const
{
    int w, h;

    this->query(nullptr, nullptr, &w, &h);

    return { pixel_type(w), pixel_type(h) };
}

void texture::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_QueryTexture(this->ptr(), format, access, w, h) == 0, ::SDL_GetError());
}

static_texture::static_texture(class window& wnd)
    : texture { wnd }
{
}

static_texture::static_texture(class window& wnd, const surface& image)
    : texture { wnd, image, SDL_TEXTUREACCESS_STATIC }
{
}

target_texture::target_texture(class window& wnd)
    : texture { wnd }
{
}

target_texture::target_texture(class window& wnd, const pixel_size& sz)
    : texture { wnd, sz, SDL_TEXTUREACCESS_TARGET }
{
}

draw& draw::rotate(lyo::f64 angle)
{
    m_angle = angle;
    return *this;
}

draw& draw::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

void draw::operator()() const
{
    if (m_this.ptr() != nullptr)
        HAL_DEBUG_ASSERT_VITAL(::SDL_RenderCopyExF(m_this.window().renderer.ptr(), m_this.ptr(), m_src.pos.x == unset<st> ? nullptr : m_src.addr(), m_dst.addr(), m_angle, nullptr, SDL_RendererFlip(m_flip))
                == 0,
            ::SDL_GetError());
}