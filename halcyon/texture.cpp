#include "texture.hpp"

#include <SDL2/SDL_image.h>

#include "debug.hpp"
#include "internal/config.hpp"
#include "window.hpp"

using namespace hal;

namespace
{
    using dest_rect = std::conditional_t<cfg::subpixel_drawing_precision, SDL_FRect, SDL_Rect>;
}

texture::texture(const window& wnd) noexcept :
    sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(), ::SDL_GetWindowPixelFormat(wnd.ptr()), SDL_TEXTUREACCESS_STATIC, 0, 0) },
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, const pixel_size& size) noexcept :
    sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(), ::SDL_GetWindowPixelFormat(wnd.ptr()), SDL_TEXTUREACCESS_STATIC, size.x, size.y) },
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, surface image) noexcept :
    sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(), image.ptr()) },
    m_size { image.size() },
    m_window { wnd }
{
}

void texture::set_opacity(lyo::u8 value) const noexcept
{
    HAL_DEBUG_VERIFY(::SDL_SetTextureAlphaMod(m_object, value) == 0, ::SDL_GetError());
}

const pixel_size& texture::size() const noexcept
{
    return m_size;
}

lyo::u8 texture::opacity() const noexcept
{
    Uint8 alpha;

    HAL_DEBUG_VERIFY(::SDL_GetTextureAlphaMod(m_object, &alpha) == 0, ::SDL_GetError());

    return alpha;
}

void texture::draw(const coordinate& pos) const noexcept
{
    this->render_copy(pos, 1.0, 0.0, none);
}

void texture::draw(const coordinate& pos, const pixel_area& src) const noexcept
{
    this->render_copy(pos, src, 1.0, 0.0, none);
}

void texture::draw(const coordinate& pos, double scale, double angle, flip_type flip) const noexcept
{
    this->render_copy(pos, scale, angle, flip);
}

void texture::draw(const coordinate& pos, const pixel_area& src, double scale, double angle, flip_type flip) const noexcept
{
    this->render_copy(pos, src, scale, angle, flip);
}

void texture::render_copy(const coordinate& pos, double scale, double angle, flip_type flip) const noexcept
{
    const world_area dest { (pos + this->size()) * scale };

    if (this->opacity() != 0 && dest | m_window.size().rect())
    {
        const dest_rect sdl_dest = dest;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_VERIFY(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object, NULL, reinterpret_cast<const SDL_FRect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_VERIFY(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object, NULL, reinterpret_cast<const SDL_Rect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}

void texture::render_copy(const coordinate& pos, const pixel_area& src, double scale, double angle, flip_type flip) const noexcept
{
    const world_area dest { (pos + src.size) * scale };

    if (this->opacity() != 0 && dest | m_window.size().rect())
    {
        const dest_rect dst_rect = dest;
        const SDL_Rect  src_rect = src;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_VERIFY(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object, &src_rect, reinterpret_cast<const SDL_FRect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_VERIFY(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object, &src_rect, reinterpret_cast<const SDL_Rect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}