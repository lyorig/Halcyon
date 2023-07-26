#include "texture.hpp"

#include "debug.hpp"
#include "internal/config.hpp"
#include "window.hpp"

using namespace hal;

namespace
{
    using dest_rect = std::conditional_t<cfg::subpixel_drawing_precision, SDL_FRect, SDL_Rect>;
}

texture::texture(const window& wnd) noexcept :
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, const pixel_size& size) noexcept :
    sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(), ::SDL_GetWindowPixelFormat(wnd.ptr()), SDL_TEXTUREACCESS_TARGET, size.x, size.y) },
    m_size { size },
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
    HAL_DEBUG_ASSERT(::SDL_SetTextureAlphaMod(m_object.get(), value) == 0, ::SDL_GetError());
}

const pixel_size& texture::size() const noexcept
{
    return m_size;
}

lyo::u8 texture::opacity() const noexcept
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT(::SDL_GetTextureAlphaMod(m_object.get(), &alpha) == 0, ::SDL_GetError());

    return alpha;
}

void texture::draw(const coordinate& pos, lyo::f64 scale, lyo::f64 angle, flip flip) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    const world_area dest { (pos + static_cast<coordinate>(this->size())) * scale };

    if (this->opacity() != 0 && dest | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect sdl_dest = dest;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object.get(), NULL, reinterpret_cast<const SDL_FRect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object.get(), NULL, reinterpret_cast<const SDL_Rect*>(&sdl_dest), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}

void texture::draw(const coordinate& pos, const pixel_area& src, lyo::f64 scale, lyo::f64 angle, flip flip) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    const world_area dest { (pos + static_cast<coordinate>(src.size)) * scale };

    if (this->opacity() != 0 && dest | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect dst_rect = dest;
        const SDL_Rect  src_rect = src;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object.get(), &src_rect, reinterpret_cast<const SDL_FRect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object.get(), &src_rect, reinterpret_cast<const SDL_Rect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}

void texture::draw(const coordinate& pos, const pixel_size& size, lyo::f64 angle, flip flip) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    const world_area dest { pos + static_cast<coordinate>(size) };

    if (this->opacity() != 0 && dest | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect dst_rect = dest;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object.get(), NULL, reinterpret_cast<const SDL_FRect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object.get(), NULL, reinterpret_cast<const SDL_Rect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}

void texture::draw(const coordinate& pos, const pixel_size& size, const pixel_area& src, lyo::f64 angle, flip flip) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    const world_area dest { pos + static_cast<coordinate>(size) };

    if (this->opacity() != 0 && dest | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect dst_rect = dest;
        const SDL_Rect  src_rect = src;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_window.renderer.ptr(), m_object.get(), &src_rect, reinterpret_cast<const SDL_FRect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(::SDL_RenderCopyEx(m_window.renderer.ptr(), m_object.get(), &src_rect, reinterpret_cast<const SDL_Rect*>(&dst_rect), angle, NULL, static_cast<SDL_RendererFlip>(flip)) == 0, ::SDL_GetError());
    }
}

void texture::draw(anchor anch, lyo::f64 scale, lyo::f64 angle, flip f) const noexcept
{
    this->draw(this->resolve_anchor(anch, m_size), scale, angle, f);
}

void texture::draw(anchor anch, const pixel_size& size, lyo::f64 angle, flip f) const noexcept
{
    this->draw(this->resolve_anchor(anch, size), size, angle, f);
}

pixel_size texture::vw(lyo::f64 percent) const noexcept
{
    const pixel_type width { static_cast<pixel_type>(m_window.size().x * (percent / 100.0)) };
    const lyo::f64   scale { width / static_cast<lyo::f64>(m_size.x) };

    return { width, static_cast<pixel_type>(m_size.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const noexcept
{
    const pixel_type height { static_cast<pixel_type>(m_window.size().y * (percent / 100.0)) };
    const lyo::f64   scale { height / static_cast<lyo::f64>(m_size.y) };

    return { static_cast<pixel_type>(m_size.x * scale), height };
}

texture& texture::operator=(surface image) noexcept
{
    this->reassign(::SDL_CreateTextureFromSurface(m_window.renderer.ptr(), image.ptr()));

    m_size = image.size();

    return *this;
}

coordinate texture::resolve_anchor(anchor anch, const pixel_size& size) const noexcept
{
    const pixel_size& win { m_window.size() };

    switch (anch)
    {
        case anchor::center:
            return { position_type(win.x / 2 - size.x / 2), position_type(win.y / 2 - size.y / 2) };

        case anchor::top_left:
            return { 0.0, 0.0 };

        case anchor::top_right:
            return { position_type(win.x - size.x), 0.0 };

        case anchor::bottom_left:
            return { 0.0, position_type(win.y - size.y) };

        case anchor::bottom_right:
            return { position_type(win.x - size.x), position_type(win.y - size.y) };

        default:
            HAL_DEBUG_PANIC("Anchor couldn't be resolved!", "Is this value in the switch statement?");
            return { 0.0, 0.0 };
    }
}

pixel_size texture::internal_size() const noexcept
{
    int w, h;
    ::SDL_QueryTexture(m_object.get(), NULL, NULL, &w, &h);
    return { static_cast<pixel_type>(w), static_cast<pixel_type>(h) };
}