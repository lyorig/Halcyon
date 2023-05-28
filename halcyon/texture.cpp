#include "texture.hpp"

#include <SDL2/SDL_image.h>

#include "debug.hpp"
#include "window.hpp"

using namespace halcyon;

texture::texture(const window& wnd) noexcept :
    sdl_object { ::SDL_CreateTexture(wnd.render, ::SDL_GetWindowPixelFormat(wnd), SDL_TEXTUREACCESS_STATIC, 0, 0), "Default texture creation failed" },
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, const pixel_size& size) noexcept :
    sdl_object { ::SDL_CreateTexture(wnd.render, ::SDL_GetWindowPixelFormat(wnd), SDL_TEXTUREACCESS_STATIC, size.x, size.y), "Sized default texture creation failed" },
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, surface image) noexcept :
    sdl_object { ::SDL_CreateTextureFromSurface(wnd.render, image), "Image-to-texture conversion failed" },
    m_size { image.size() },
    m_window { wnd }
{
}

void texture::set_opacity(lyo::u8 value) const noexcept
{
    HALCYON_VERIFY(::SDL_SetTextureAlphaMod(m_object, value) == 0, "Couldn't set texture opacity");
}

const pixel_size& texture::size() const noexcept
{
    return m_size;
}

lyo::u8 texture::opacity() const noexcept
{
    Uint8 alpha;

    HALCYON_VERIFY(::SDL_GetTextureAlphaMod(m_object, &alpha) == 0, "Couldn't get texture alpha mod");

    return alpha;
}

void texture::draw(const coordinate& pos) const noexcept
{
    this->render_copy(pos, std::nullopt, 1.0, 0.0, none);
}

void texture::draw(const coordinate& pos, const pixel_area& src) const noexcept
{
    this->render_copy(pos, src, 1.0, 0.0, none);
}

void texture::draw(const coordinate& pos, double scale, double angle, flip_t flip) const noexcept
{
    this->render_copy(pos, std::nullopt, scale, angle, flip);
}

void texture::draw(const coordinate& pos, const pixel_area& src, double scale, double angle, flip_t flip) const noexcept
{
    this->render_copy(pos, src, scale, angle, flip);
}

void texture::render_copy(const coordinate& pos, std::optional<pixel_area> src, double scale, double angle, flip_t flip) const noexcept
{
    using dest_t = std::conditional_t<float_draw, SDL_FRect, SDL_Rect>;

    const world_area dest { (pos + (src ? src->size : this->size())) * scale };

    if (this->opacity() != 0 && dest | m_window.size().rect())
    {
        const dest_t sdl_dest = dest;

        if (!src)
        {
            if constexpr (float_draw)
                HALCYON_VERIFY(::SDL_RenderCopyExF(m_window.render, m_object, NULL, reinterpret_cast<const SDL_FRect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, "Texture drawing (floating-point) failed");

            else
                HALCYON_VERIFY(::SDL_RenderCopyEx(m_window.render, m_object, NULL, reinterpret_cast<const SDL_Rect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, "Texture drawing (integral) failed");
        }

        else
        {
            SDL_Rect src_rect = *src;

            if constexpr (float_draw)
                HALCYON_VERIFY(::SDL_RenderCopyExF(m_window.render, m_object, &src_rect, reinterpret_cast<const SDL_FRect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, "Texture drawing (floating-point) failed");

            else
                HALCYON_VERIFY(::SDL_RenderCopyEx(m_window.render, m_object, &src_rect, reinterpret_cast<const SDL_Rect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, "Texture drawing (integral) failed");
        }
    }
}