#include <halcyon/video/renderer.hpp>

#include <halcyon/surface.hpp>

#include <halcyon/video/texture.hpp>
#include <halcyon/video/window.hpp>

#include <halcyon/internal/helpers.hpp>

using namespace hal;

renderer::renderer(const hal::window& wnd, std::initializer_list<flags> f, pass_key<window>)
    : raii_object { ::SDL_CreateRenderer(wnd.get(), -1, detail::to_bitmask<std::uint32_t>(f)) }
{
    HAL_PRINT("Created renderer for \"", wnd.title(), "\" ", info());
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

void renderer::target(target_texture& tx)
{
    this->internal_target(tx.get());
}

void renderer::reset_target()
{
    this->internal_target(nullptr);
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

info::renderer renderer::info() const
{
    return { *this, pass_key<renderer> {} };
}

texture renderer::make_texture(const surface& surf) &
{
    return { *this, surf, pass_key<renderer> {} };
}

target_texture renderer::make_target_texture(pixel_point size) &
{
    SDL_Window* wnd { ::SDL_RenderGetWindow(get()) };
    HAL_ASSERT(wnd != nullptr, debug::last_error());

    const SDL_PixelFormatEnum fmt { static_cast<SDL_PixelFormatEnum>(::SDL_GetWindowPixelFormat(wnd)) };
    HAL_ASSERT(fmt != SDL_PIXELFORMAT_UNKNOWN, debug::last_error());

    return { *this, fmt, size, pass_key<renderer> {} };
}

color renderer::color() const
{
    hal::color ret;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawColor(get(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, debug::last_error());

    return ret;
}

void renderer::color(hal::color clr)
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

copyer renderer::draw(const detail::texture_base& tex)
{
    return { *this, tex, pass_key<renderer> {} };
}

void renderer::internal_target(SDL_Texture* target)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderTarget(get(), target) == 0, debug::last_error());
}

// Renderer information.

info::renderer::renderer(const hal::renderer& rnd, pass_key<hal::renderer>)
{
    HAL_ASSERT_VITAL(::SDL_GetRendererInfo(rnd.get(), static_cast<SDL_RendererInfo*>(this)) == 0, debug::last_error());
}

std::string_view info::renderer::name() const
{
    return SDL_RendererInfo::name;
}

info::renderer::flag_bitset info::renderer::flags() const
{
    return SDL_RendererInfo::flags;
}

pixel_point info::renderer::max_texture_size() const
{
    return {
        static_cast<pixel_t>(max_texture_width),
        static_cast<pixel_t>(max_texture_height)
    };
}

// Copyer.

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
    HAL_ASSERT_VITAL(::SDL_RenderCopyExF(m_pass.get(), m_this.get(),
                         m_src.pos.x == unset_pos<src_t>() ? nullptr : reinterpret_cast<const SDL_Rect*>(m_src.addr()),
                         m_dst.pos.x == unset_pos<dst_t>() ? nullptr : reinterpret_cast<const SDL_FRect*>(m_dst.addr()),
                         m_angle, nullptr, static_cast<SDL_RendererFlip>(m_flip))
            == 0,
        debug::last_error());
}