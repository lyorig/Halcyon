#include <halcyon/renderer.hpp>

#include <halcyon/internal/helpers.hpp>
#include <halcyon/window.hpp>

using namespace hal;

renderer::renderer(window& wnd, std::initializer_list<flags> flags)
    : object { ::SDL_CreateRenderer(wnd.ptr(), -1, il2bm<Uint32>(flags)) }
{
    this->set_blend(blend_mode::blend);
}

void renderer::present()
{
    ::SDL_RenderPresent(this->ptr());
    this->clear();
}

void renderer::clear()
{
    HAL_ASSERT_VITAL(::SDL_RenderClear(this->ptr()) == 0, ::SDL_GetError());
}

void renderer::draw_line(const sdl::coord_point& from, const sdl::coord_point& to)
{
    if constexpr (sdl::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderDrawLine(this->ptr(), static_cast<sdl::coord_t>(from.x),
                             static_cast<sdl::coord_t>(from.y),
                             static_cast<sdl::coord_t>(to.x),
                             static_cast<sdl::coord_t>(to.y))
                == 0,
            ::SDL_GetError());
    }

    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderDrawLineF(this->ptr(), static_cast<sdl::coord_t>(from.x),
                             static_cast<sdl::coord_t>(from.y),
                             static_cast<sdl::coord_t>(to.x),
                             static_cast<sdl::coord_t>(to.y))
                == 0,
            ::SDL_GetError());
    }
}

void renderer::draw_rect(const sdl::coord_rect& area)
{
    if constexpr (sdl::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderDrawRect(this->ptr(), reinterpret_cast<const SDL_Rect*>(area.addr())) == 0, ::SDL_GetError());
    }

    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderDrawRectF(this->ptr(), reinterpret_cast<const SDL_FRect*>(area.addr())) == 0, ::SDL_GetError());
    }
}

void renderer::fill_rect(const sdl::coord_rect& area)
{
    if constexpr (sdl::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderFillRect(this->ptr(), reinterpret_cast<const SDL_Rect*>(area.addr())) == 0, ::SDL_GetError());
    }
    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderFillRectF(this->ptr(), reinterpret_cast<const SDL_FRect*>(area.addr())) == 0, ::SDL_GetError());
    }
}

void renderer::fill_rects(const std::span<const sdl::coord_rect>& areas)
{
    if constexpr (sdl::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderFillRects(ptr(), reinterpret_cast<const SDL_Rect*>(areas.data()), areas.size()) == 0, ::SDL_GetError());
    }
    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderFillRectsF(ptr(), reinterpret_cast<const SDL_FRect*>(areas.data()), areas.size()) == 0, ::SDL_GetError());
    }
}

void renderer::fill_target()
{
    // Coord types aren't relevant here.
    HAL_ASSERT_VITAL(::SDL_RenderFillRect(this->ptr(), nullptr) == 0, ::SDL_GetError());
}

pixel_point renderer::size() const
{
    point<int> sz;
    ::SDL_RenderGetLogicalSize(this->ptr(), &sz.x, &sz.y);

    if (sz.x == 0)
        ::SDL_GetRendererOutputSize(this->ptr(), &sz.x, &sz.y);

    return sz;
}

void renderer::set_size(const pixel_point& sz)
{
    HAL_ASSERT_VITAL(::SDL_RenderSetLogicalSize(this->ptr(), sz.x, sz.y) == 0, ::SDL_GetError());
}

void renderer::set_target(target_texture& tx)
{
    this->internal_set_target(tx.ptr());
}

void renderer::reset_target()
{
    this->internal_set_target(nullptr);
}

color renderer::draw_color() const
{
    color ret;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawColor(this->ptr(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, ::SDL_GetError());

    return ret;
}

void renderer::set_draw_color(color clr)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawColor(this->ptr(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}

blend_mode renderer::blend() const
{
    SDL_BlendMode bm;

    HAL_ASSERT_VITAL(::SDL_GetRenderDrawBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void renderer::set_blend(blend_mode bm)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

copyer renderer::draw(const texture_base& tex)
{
    return { *this, tex, {} };
}

void renderer::internal_render_copy(const texture_base& tex, const sdl::pixel_rect* src, const sdl::coord_rect* dst, lyo::f64 angle, flip f, lyo::pass_key<copyer>)
{
    HAL_ASSERT(tex.exists(), "Drawing null texture");

    if constexpr (sdl::integral_coord)
    {
        HAL_ASSERT_VITAL(::SDL_RenderCopyEx(this->ptr(), tex.ptr(), reinterpret_cast<const SDL_Rect*>(src), reinterpret_cast<const SDL_Rect*>(dst), angle, NULL, SDL_RendererFlip(f)) == 0, ::SDL_GetError());
    }
    else
    {
        HAL_ASSERT_VITAL(::SDL_RenderCopyExF(this->ptr(), tex.ptr(), reinterpret_cast<const SDL_Rect*>(src), reinterpret_cast<const SDL_FRect*>(dst), angle, NULL, SDL_RendererFlip(f)) == 0, ::SDL_GetError());
    }
}

void renderer::internal_set_target(SDL_Texture* target)
{
    HAL_ASSERT_VITAL(::SDL_SetRenderTarget(this->ptr(), target) == 0, ::SDL_GetError());
}

color_lock::color_lock(renderer& rnd)
    : m_rnd { rnd }
    , m_old { rnd.draw_color() }
{
}

color_lock::color_lock(renderer& rnd, color new_clr)
    : color_lock { rnd }
{
    set(new_clr);
}

color_lock::~color_lock()
{
    m_rnd.set_draw_color(m_old);
}

void color_lock::set(color clr)
{
    m_rnd.set_draw_color(clr);
}

target_lock::target_lock(renderer& rnd, target_texture& tgt)
    : m_rnd { rnd }
{
    set(tgt);
}

target_lock::~target_lock()
{
    m_rnd.reset_target();
}

void target_lock::set(target_texture& tgt)
{
    m_rnd.set_target(tgt);
}