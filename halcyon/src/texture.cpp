#include <halcyon/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/window.hpp>

using namespace hal;

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

texture::texture(SDL_Texture* ptr)
    : sdl_object { ptr }
{
}

pixel_size texture::internal_size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void texture::query(Uint32* format, int* access, int* w, int* h) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_QueryTexture(this->ptr(), format, access, w, h) == 0, ::SDL_GetError());
}
static_texture::static_texture(window& wnd, const surface& image)
    : texture { create(wnd, image) }
{
}

static_texture& static_texture::change(window& wnd, const surface& image)
{
    sdl_object::reset(create(wnd, image));
    return *this;
}

SDL_Texture* static_texture::create(window& wnd, const surface& image)
{
    return ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(), image.ptr());
}

target_texture::target_texture(window& wnd, const pixel_size& sz)
    : texture { create(wnd, sz) }
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