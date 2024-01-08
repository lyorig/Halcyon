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
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderClear(this->ptr()) == 0, ::SDL_GetError());
}

void renderer::draw_line(const coord& from, const coord& to)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderDrawLineF(this->ptr(), static_cast<SDL::coord_t>(from.x),
                               static_cast<SDL::coord_t>(from.y),
                               static_cast<SDL::coord_t>(to.x),
                               static_cast<SDL::coord_t>(to.y))
            == 0,
        ::SDL_GetError());
}

void renderer::draw_rect(const coord_area& area)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderDrawRectF(this->ptr(), SDL::FRect(area).addr()) == 0, ::SDL_GetError());
}

void renderer::fill_rect(const SDL::FRect& area)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderFillRectF(this->ptr(), area.addr()) == 0, ::SDL_GetError());
}

void renderer::fill_target()
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderFillRectF(this->ptr(), nullptr) == 0, ::SDL_GetError());
}

void renderer::set_logical_size(const pixel_size& sz)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderSetLogicalSize(this->ptr(), sz.x, sz.y) == 0, ::SDL_GetError());
}

pixel_size renderer::output_size() const
{
    point<int> size;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRendererOutputSize(this->ptr(), &size.x, &size.y) == 0, ::SDL_GetError());

    return size;
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

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRenderDrawColor(this->ptr(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, ::SDL_GetError());

    return ret;
}

void renderer::set_draw_color(color clr)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawColor(this->ptr(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}

blend_mode renderer::blend() const
{
    SDL_BlendMode bm;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRenderDrawBlendMode(this->ptr(), &bm) == 0, ::SDL_GetError());

    return blend_mode(bm);
}

void renderer::set_blend(blend_mode bm)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(this->ptr(), SDL_BlendMode(bm)) == 0, ::SDL_GetError());
}

void renderer::internal_render_copy(const texture_base& tex, const SDL_Rect* src, const SDL_FRect* dst, lyo::f64 angle, flip f, lyo::pass_key<draw>)
{
    HAL_DEBUG_ASSERT(tex.ptr() != nullptr, "Drawing null texture");

    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderCopyExF(this->ptr(), tex.ptr(), src, dst, angle, NULL, SDL_RendererFlip(f)) == 0, ::SDL_GetError());
}

void renderer::internal_set_target(SDL_Texture* target)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(this->ptr(), target) == 0, ::SDL_GetError());
}

color_lock::color_lock(renderer& rnd, color new_clr)
    : m_rnd { rnd }
    , m_old { rnd.draw_color() }
{
    m_rnd.set_draw_color(new_clr);
}

color_lock::~color_lock()
{
    m_rnd.set_draw_color(m_old);
}

target_lock::target_lock(renderer& rnd, target_texture& tgt)
    : m_rnd { rnd }
{
    m_rnd.set_target(tgt);
}

target_lock::~target_lock()
{
    m_rnd.reset_target();
}