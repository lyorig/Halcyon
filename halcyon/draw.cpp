#include "draw.hpp"
#include <halcyon/window.hpp>

using namespace hal;

drawer::drawer(lyo::pass_key<texture>, const texture& tex) noexcept
    : m_src { pixel_size {}.rect(tex.size()) }
    , m_tex { tex }
{
}

drawer& drawer::from(const pixel_area& src) noexcept
{
    m_src = src;

    return *this;
}

drawer& drawer::to(const coordinate& dst) noexcept
{
    m_dst = dst;
    m_dstSize = std::nullopt;

    return *this;
}

drawer& drawer::to(const world_area& dst) noexcept
{
    m_dst = dst.pos;
    m_dstSize = static_cast<pixel_size>(dst.size);

    return *this;
}

drawer& drawer::scale(lyo::f64 scl) noexcept
{
    m_scale = scl;

    return *this;
}

drawer& drawer::rotate_around(const coordinate& center) noexcept
{
    m_rot = center;

    return *this;
}

drawer& drawer::rotate(lyo::f64 angle) noexcept
{
    m_scale = angle;

    return *this;
}

drawer& drawer::anchor(hal::anchor anch) noexcept
{
    m_anch = anch;

    return *this;
}

drawer& drawer::flip(enum flip f) noexcept
{
    m_flip = f;
}

void drawer::operator()() const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_RenderCopyExF(m_tex.window.renderer.ptr(), m_tex.ptr(), m_src ? &*m_src : nullptr, &dst, m_angle, m_rot ? &*m_rot : nullptr, static_cast<SDL_RendererFlip>(m_flip)) == 0, ::SDL_GetError());
}

constexpr world_area drawer::resolve_anchor() const noexcept
{
    using p = position_type;

    return {};
}