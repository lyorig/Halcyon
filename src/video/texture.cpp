#include <halcyon/video/texture.hpp>

#include <halcyon/debug.hpp>
#include <halcyon/surface.hpp>
#include <halcyon/video/renderer.hpp>

using namespace hal;

hal::pixel_point hal::detail::texture_base::size() const
{
    point<int> size;

    this->query(nullptr, nullptr, &size.x, &size.y);

    return size;
}

void hal::detail::texture_base::opacity(color::value_t value)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureAlphaMod(get(), value) == 0, debug::last_error());
}

hal::color::value_t hal::detail::texture_base::alpha_mod() const
{
    hal::color::value_t ret;

    ::SDL_GetTextureAlphaMod(get(), &ret);

    return ret;
}

void hal::detail::texture_base::alpha_mod(hal::color::value_t val)
{
    ::SDL_SetTextureAlphaMod(get(), val);
}

hal::color hal::detail::texture_base::color_mod() const
{
    color c;

    HAL_ASSERT_VITAL(::SDL_GetTextureColorMod(get(), &c.r, &c.g, &c.b) == 0, debug::last_error());

    return c;
}

void hal::detail::texture_base::color_mod(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureColorMod(get(), clr.r, clr.g, clr.b) == 0, debug::last_error());
}

hal::blend_mode hal::detail::texture_base::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetTextureBlendMode(get(), &bm) == 0, debug::last_error());

    return blend_mode(bm);
}

void hal::detail::texture_base::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetTextureBlendMode(get(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

hal::u8 hal::detail::texture_base::opacity() const
{
    Uint8 alpha;

    HAL_ASSERT_VITAL(::SDL_GetTextureAlphaMod(get(), &alpha) == 0,
        debug::last_error());

    return alpha;
}

hal::detail::texture_base::texture_base(SDL_Texture* ptr)
    : raii_object { ptr }
{
    this->blend(blend_mode::blend);
}

void hal::detail::texture_base::query(std::uint32_t* format, int* access, int* w, int* h) const
{
    HAL_ASSERT_VITAL(::SDL_QueryTexture(get(), format, access, w, h) == 0, debug::last_error());
}

texture::texture(SDL_Texture* ptr, pass_key<renderer>)
    : texture_base { ptr }
{
}

target_texture::target_texture(SDL_Texture* ptr, pass_key<renderer>)
    : texture_base { ptr }
{
}