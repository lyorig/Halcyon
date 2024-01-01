#include <halcyon/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/window.hpp>

using namespace hal;

pixel_size texture_base::size() const
{
    return this->internal_size();
}

void texture_base::set_opacity(color::value value)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetTextureAlphaMod(this->ptr(), value) == 0, ::SDL_GetError());
}

color texture_base::color_mod() const
{
    color c;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetTextureColorMod(this->ptr(), &c.r, &c.g, &c.b) == 0, ::SDL_GetError());

    return c;
}

void texture_base::set_color_mod(color clr)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetTextureColorMod(this->ptr(), clr.r, clr.g, clr.b) == 0, ::SDL_GetError());
}

blend_mode texture_base::blend() const
{
    SDL_BlendMode bm;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetTextureBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void texture_base::set_blend(blend_mode bm)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetTextureBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

lyo::u8 texture_base::opacity() const
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetTextureAlphaMod(this->ptr(), &alpha) == 0,
        ::SDL_GetError());

    return alpha;
}

texture_base::texture_base(SDL_Texture* ptr)
    : sdl_object { ptr }
{
    this->set_blend(blend_mode::blend);
}

pixel_size texture_base::internal_size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void texture_base::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_QueryTexture(this->ptr(), format, access, w, h) == 0, ::SDL_GetError());
}

texture::texture(window& wnd, const surface& image)
    : texture_base { create(wnd, image) }
{
}

texture& texture::change(window& wnd, const surface& image)
{
    sdl_object::reset(create(wnd, image));
    return *this;
}

SDL_Texture* texture::create(window& wnd, const surface& image)
{
    return ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(), image.ptr());
}

target_texture::target_texture(window& wnd, const pixel_size& sz)
    : texture_base { create(wnd, sz) }
{
}

void target_texture::resize(window& wnd, const pixel_size& sz)
{
    sdl_object::reset(create(wnd, sz));
}

SDL_Texture* target_texture::create(window& wnd, const pixel_size& sz)
{
    const Uint32 pixel_format { ::SDL_GetWindowPixelFormat(wnd.ptr()) };
    HAL_DEBUG_ASSERT(pixel_format != SDL_PIXELFORMAT_UNKNOWN, ::SDL_GetError());

    SDL_Texture* tex { ::SDL_CreateTexture(wnd.renderer.ptr(), pixel_format, SDL_TEXTUREACCESS_TARGET, sz.x, sz.y) };
    HAL_DEBUG_ASSERT(tex != nullptr, ::SDL_GetError());

    return tex;
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

void draw::operator()(window& wnd) const
{
    wnd.renderer.internal_render_copy(
        m_this,
        m_src.pos.x == unset<st> ? nullptr : m_src.addr(),
        m_dst.pos.x == unset<dt> ? nullptr : m_dst.addr(),
        m_angle,
        m_flip,
        {});
}