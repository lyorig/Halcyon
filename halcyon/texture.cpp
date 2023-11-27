#include "texture.hpp"

#include "debug.hpp"
#include "halcyon/types/render.hpp"
#include "internal/config.hpp"
#include "window.hpp"

using namespace hal;

texture::texture(const window& wnd) noexcept :
    m_size { 0, 0 },
    m_window { wnd }
{
}

texture::texture(const window& wnd, const pixel_size& size) noexcept
    :
    sdl_object { ::SDL_CreateTexture(wnd.renderer.ptr(),
        ::SDL_GetWindowPixelFormat(wnd.ptr()),
        SDL_TEXTUREACCESS_TARGET, size.x, size.y) },
    m_size { size },
    m_window { wnd }
{
}

texture::texture(const window& wnd, const surface& image) noexcept
    :
    sdl_object { ::SDL_CreateTextureFromSurface(wnd.renderer.ptr(),
        image.ptr()) },
    m_size { image.size() },
    m_window { wnd }
{
}

void texture::set_opacity(lyo::u8 value) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetTextureAlphaMod(m_object.get(), value) == 0,
        ::SDL_GetError());
}

const pixel_size& texture::size() const noexcept { return m_size; }

lyo::u8 texture::opacity() const noexcept
{
    Uint8 alpha;

    HAL_DEBUG_ASSERT(::SDL_GetTextureAlphaMod(m_object.get(), &alpha) == 0,
        ::SDL_GetError());

    return static_cast<lyo::u8>(alpha);
}

// Draw the entire texture to a position.
void texture::draw(const coordinate& pos, lyo::f64 scale, lyo::f64 angle,
    flip f) const noexcept
{
    this->render_copy(pos.rect(static_cast<coordinate>(m_size) * scale), angle,
        f);
}

void texture::draw(const coordinate& pos, const pixel_size& size,
    lyo::f64 angle, flip f) const noexcept
{
    this->render_copy(pos.rect(static_cast<coordinate>(size)), angle, f);
}

// Draw a part of a texture to a position.
void texture::draw(const coordinate& pos, const pixel_area& src, lyo::f64 scale,
    lyo::f64 angle, flip f) const noexcept
{
    this->render_copy(pos.rect(static_cast<coordinate>(src.size) * scale), src,
        angle, f);
}

// Draw the entire texture to a destination rectangle.
void texture::draw(const world_area& dest, lyo::f64 angle,
    flip f) const noexcept
{
    this->render_copy(dest, angle, f);
}

// Draw a part of a texture to a destination rectangle.
void texture::draw(const world_area& dest, const pixel_area& src,
    lyo::f64 angle, flip f) const noexcept
{
    this->render_copy(dest, src, angle, f);
}

// Draw the entire texture and anchor it to the current renderer output.
void texture::draw(anchor anch, lyo::f64 scale, lyo::f64 angle,
    flip f) const noexcept
{
    this->draw(this->resolve_anchor(anch,
                   static_cast<world_area>(
                       m_window.renderer.output_size().rect()),
                   m_size * scale),
        scale, angle, f);
}

// Draw the entire texture to a specific anchor point.
void texture::draw(anchor anch, const pixel_size& size, lyo::f64 angle,
    flip f) const noexcept
{
    this->draw(this->resolve_anchor(anch,
                   static_cast<world_area>(
                       m_window.renderer.output_size().rect()),
                   size),
        size, angle, f);
}

// Draw the entire texture to an anchored position.
void texture::draw(const coordinate& pos, anchor anch, lyo::f64 angle,
    flip f) const noexcept
{
    this->draw(this->resolve_anchor(anch, pos, m_size), m_size, angle, f);
}

void texture::set_as_target() noexcept { m_window.renderer.set_target(*this); }

pixel_size texture::vw(lyo::f64 percent) const noexcept
{
    const pixel_type width { static_cast<pixel_type>(
        m_window.renderer.output_size().x * (percent / 100.0)) };
    const lyo::f64   scale { width / static_cast<lyo::f64>(m_size.x) };

    return { width, static_cast<pixel_type>(m_size.y * scale) };
}

pixel_size texture::vh(lyo::f64 percent) const noexcept
{
    const pixel_type height {
        static_cast<pixel_type>(m_window.size().y * (percent / 100.0))
    };
    const lyo::f64 scale { height / static_cast<lyo::f64>(m_size.y) };

    return { static_cast<pixel_type>(m_size.x * scale), height };
}

texture& texture::operator=(const surface& image) noexcept
{
    this->reassign(
        ::SDL_CreateTextureFromSurface(m_window.renderer.ptr(), image.ptr()));

    m_size = image.size();

    return *this;
}

void texture::render_copy(const world_area& dst, lyo::f64 angle,
    flip f) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    if (this->opacity() > 0 &&
        dst | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect dst_rect = dst;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(
                                 m_window.renderer.ptr(), m_object.get(), NULL,
                                 reinterpret_cast<const SDL_FRect*>(&dst_rect),
                                 angle, NULL,
                                 static_cast<SDL_RendererFlip>(f)) == 0,
                ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(::SDL_RenderCopyEx(
                                 m_window.renderer.ptr(), m_object.get(), NULL,
                                 reinterpret_cast<const SDL_Rect*>(&dst_rect),
                                 angle, NULL,
                                 static_cast<SDL_RendererFlip>(f)) == 0,
                ::SDL_GetError());
    }
}

void texture::render_copy(const world_area& dst, const pixel_area& src,
    lyo::f64 angle, flip f) const noexcept
{
    HAL_DEBUG_CHECK(m_object, "Drawing null texture");

    if (this->opacity() > 0 &&
        dst | static_cast<coordinate>(m_window.size()).rect())
    {
        const dest_rect dst_rect = dst;
        const SDL_Rect  src_rect = src;

        if constexpr (cfg::subpixel_drawing_precision)
            HAL_DEBUG_ASSERT(
                ::SDL_RenderCopyExF(
                    m_window.renderer.ptr(), m_object.get(), &src_rect,
                    reinterpret_cast<const SDL_FRect*>(&dst_rect), angle, NULL,
                    static_cast<SDL_RendererFlip>(f)) == 0,
                ::SDL_GetError());

        else
            HAL_DEBUG_ASSERT(
                ::SDL_RenderCopyEx(
                    m_window.renderer.ptr(), m_object.get(), &src_rect,
                    reinterpret_cast<const SDL_Rect*>(&dst_rect), angle, NULL,
                    static_cast<SDL_RendererFlip>(f)) == 0,
                ::SDL_GetError());
    }
}

constexpr coordinate
texture::resolve_anchor(anchor anch, const coordinate& pos,
    const pixel_size& size) const noexcept
{
    using p = position_type;

    switch (anch)
    {
        case anchor::center:
            return { position_type(pos.x - size.x / p { 2 }),
                position_type(pos.y - size.y / p { 2 }) };

        case anchor::top_left:
            return pos;

        case anchor::top_right:
            return { position_type(pos.x - size.x), pos.y };

        case anchor::bottom_left:
            return { pos.x, position_type(pos.y - size.y) };

        case anchor::bottom_right:
            return { position_type(pos.x - size.x), position_type(pos.y - size.y) };

        default:
            HAL_DEBUG_PANIC("Anchor couldn't be resolved!",
                "Is this value in the switch statement?");
            return {};
    }
}

constexpr coordinate
texture::resolve_anchor(anchor anch, const world_area& dest,
    const pixel_size& size) const noexcept
{
    using p = position_type;

    switch (anch)
    {
        case anchor::center:
            return { position_type(dest.pos.x + dest.size.x / p { 2 } - size.x / p { 2 }),
                position_type(dest.pos.y + dest.size.y / p { 2 } - size.y / p { 2 }) };

        case anchor::top_left:
            return dest.pos;

        case anchor::top_right:
            return { position_type(dest.pos.x + dest.size.x - size.x), dest.pos.y };

        case anchor::bottom_left:
            return { dest.pos.x, position_type(dest.pos.y + dest.size.y - size.y) };

        case anchor::bottom_right:
            return { position_type(dest.pos.x + dest.size.x - size.x),
                position_type(dest.pos.y + dest.size.y - size.y) };

        default:
            HAL_DEBUG_PANIC("Anchor couldn't be resolved!",
                "Is this value in the switch statement?");
            return {};
    }
}

pixel_size texture::internal_size() const noexcept
{
    int w, h;

    ::SDL_QueryTexture(m_object.get(), NULL, NULL, &w, &h);

    return { static_cast<pixel_type>(w), static_cast<pixel_type>(h) };
}