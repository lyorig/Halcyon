#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

renderer::color_lock::color_lock(renderer& rnd, color new_clr)
    : m_rnd { rnd }
    , m_old { rnd.draw_color() }
{
    set(new_clr);
}

renderer::color_lock::~color_lock()
{
    m_rnd.draw_color(m_old);
}

void renderer::color_lock::set(color clr)
{
    m_rnd.draw_color(clr);
}

renderer::target_lock::target_lock(renderer& rnd, target_texture& tgt)
    : m_rnd { rnd }
{
    set(tgt);
}

renderer::target_lock::~target_lock()
{
    m_rnd.retarget();
}

void renderer::target_lock::set(target_texture& tgt)
{
    m_rnd.target(tgt);
}

renderer::renderer(SDL_Renderer* ptr, pass_key<window>)
    : raii_object { ptr }
{
}

void renderer::present()
{
    ::SDL_RenderPresent(get());
    this->clear();
}

void renderer::clear()
{
    HAL_ASSERT_VITAL(::SDL_RenderClear(get()) == 0, debug::last_error());
}

void renderer::draw_point(const sdl::coord_point& pt)
{
#ifdef HAL_INTEGRAL_COORD
    ::SDL_RenderDrawPoint(get(), pt.x, pt.y);
#else
    ::SDL_RenderDrawPointF(get(), pt.x, pt.y);
#endif
}

void renderer::draw_line(const sdl::coord_point& from, const sdl::coord_point& to)
{
#ifdef HAL_INTEGRAL_COORD
    HAL_ASSERT_VITAL(::SDL_RenderDrawLine(get(), from.x, from.y, to.x, to.y) == 0, debug::last_error());
#else
    HAL_ASSERT_VITAL(::SDL_RenderDrawLineF(get(), from.x, from.y, to.x, to.y) == 0, debug::last_error());
#endif
}

void renderer::draw_rect(const sdl::coord_rect& area)
{
#ifdef HAL_INTEGRAL_COORD
    HAL_ASSERT_VITAL(::SDL_RenderDrawRect(get(), area.addr()) == 0, debug::last_error());
#else
    HAL_ASSERT_VITAL(::SDL_RenderDrawRectF(get(), area.addr()) == 0, debug::last_error());
#endif
}

void renderer::fill_rect(const sdl::coord_rect& area)
{
#ifdef HAL_INTEGRAL_COORD
    HAL_ASSERT_VITAL(::SDL_RenderFillRect(get(), area.addr()) == 0, debug::last_error());
#else
    HAL_ASSERT_VITAL(::SDL_RenderFillRectF(get(), area.addr()) == 0, debug::last_error());
#endif
}

void renderer::fill_rects(const std::span<const sdl::coord_rect>& areas)
{
#ifdef HAL_INTEGRAL_COORD
    HAL_ASSERT_VITAL(::SDL_RenderFillRects(get(), areas.front().addr(), static_cast<int>(areas.size())) == 0, debug::last_error());
#else
    HAL_ASSERT_VITAL(::SDL_RenderFillRectsF(get(), areas.front().addr(), static_cast<int>(areas.size())) == 0, debug::last_error());
#endif
}

void renderer::fill_target()
{
    // Coord types aren't relevant here.
    HAL_ASSERT_VITAL(::SDL_RenderFillRect(get(), nullptr) == 0, debug::last_error());
}

pixel_point renderer::size() const
{
    point<int> sz;
    ::SDL_RenderGetLogicalSize(get(), &sz.x, &sz.y);

    if (sz.x == 0)
        HAL_ASSERT_VITAL(::SDL_GetRendererOutputSize(get(), &sz.x, &sz.y) == 0, debug::last_error());

    return static_cast<pixel_point>(sz);
}

void renderer::size(pixel_point sz)
{
    HAL_ASSERT_VITAL(::SDL_RenderSetLogicalSize(get(), sz.x, sz.y) == 0, debug::last_error());
}

void renderer::size(scaler scl)
{
    size(scl(size()));
}

void renderer::target(target_texture& tx)
{
    this->internal_target(tx.get());
}

void renderer::retarget()
{
    this->internal_target(nullptr);
}

color renderer::draw_color() const
{
    color ret;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawColor(get(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, debug::last_error());

    return ret;
}

void renderer::draw_color(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawColor(get(), clr.r, clr.g, clr.b, clr.a) == 0, debug::last_error());
}

blend_mode renderer::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawBlendMode(get(), &bm) == 0, debug::last_error());

    return static_cast<blend_mode>(bm);
}

void renderer::blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(get(), SDL_BlendMode(bm)) == 0, debug::last_error());
}

texture renderer::make_texture(const surface& surf) &
{
    return { ::SDL_CreateTextureFromSurface(get(), surf.get()), pass_key<renderer> {} };
}

target_texture renderer::make_target_texture(pixel_point size) &
{
    SDL_Window* wnd { ::SDL_RenderGetWindow(get()) };
    HAL_ASSERT(wnd != nullptr, debug::last_error());

    const std::uint32_t fmt { ::SDL_GetWindowPixelFormat(wnd) };
    HAL_ASSERT(fmt != SDL_PIXELFORMAT_UNKNOWN, debug::last_error());

    return { ::SDL_CreateTexture(get(), fmt, SDL_TEXTUREACCESS_STATIC, size.x, size.y), pass_key<renderer> {} };
}

copyer renderer::draw(const detail::texture_base& tex)
{
    return { *this, tex, pass_key<renderer> {} };
}

void renderer::internal_target(SDL_Texture* target)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderTarget(get(), target) == 0, debug::last_error());
}

copyer& copyer::rotate(f64 angle)
{
    m_angle = angle;
    return *this;
}

copyer& copyer::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

copyer& copyer::outline()
{
    m_pass.draw_rect(m_dst);
    return *this;
}

void copyer::operator()()
{
    if constexpr (compile_settings::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderCopyEx(m_pass.get(), m_this.get(),
                             m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                             m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_dst.addr()),
                             m_angle,
                             nullptr, static_cast<SDL_RendererFlip>(m_flip))
                == 0,
            debug::last_error());
    }

    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderCopyExF(m_pass.get(), m_this.get(),
                             m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                             m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<const SDL_FRect*>(m_dst.addr()),
                             m_angle, nullptr, static_cast<SDL_RendererFlip>(m_flip))
                == 0,
            debug::last_error());
    }
}