#include <halcyon/components/renderer.hpp>

#include <halcyon/window.hpp>

using namespace hal;

draw& draw::rotate(lyo::f64 angle)
{
    m_angle = angle;
    return *this;
}

draw& draw::flip(enum flip f)
{
    m_flip = f;
    return *this;
}

void draw::operator()(window& wnd) const
{
    if (m_this.ptr() != nullptr)
        HAL_DEBUG_ASSERT_VITAL(::SDL_RenderCopyExF(wnd.renderer.ptr(), m_this.ptr(), m_src.pos.x == unset<st> ? nullptr : m_src.addr(), m_dst.addr(), m_angle, nullptr, SDL_RendererFlip(m_flip))
                == 0,
            ::SDL_GetError());
}

draw_hijack::draw_hijack(window& wnd, color new_clr)
    : draw_hijack { wnd.renderer, new_clr }
{
}

draw_hijack::draw_hijack(renderer& rnd, color new_clr)
    : m_rnd { rnd }
    , m_old { rnd.get_color() }
{
    m_rnd.set_color(new_clr);
}

draw_hijack::~draw_hijack()
{
    m_rnd.set_color(m_old);
}

target_hijack::target_hijack(window& wnd, target_texture& tgt)
    : target_hijack { wnd.renderer, tgt }
{
}

target_hijack::target_hijack(renderer& rnd, target_texture& tgt)
    : m_rnd { rnd }
{
    m_rnd.set_target(tgt);
}

target_hijack::~target_hijack()
{
    m_rnd.reset_target();
}

renderer::renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>)
    : sdl_object { ::SDL_CreateRenderer(wnd.ptr(), -1, flags) }
{
}

void renderer::present(lyo::pass_key<window>) const
{
    ::SDL_RenderPresent(this->ptr());
}

void renderer::clear(lyo::pass_key<window>) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderClear(this->ptr()) == 0, ::SDL_GetError());
}

void renderer::draw_line(const coord& from, const coord& to, color clr)
{
    const draw_hijack dh { *this, clr };
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderDrawLineF(this->ptr(), from.x, from.y, to.x, to.y) == 0, ::SDL_GetError());
}

void renderer::draw_rect(const coord_area& area, color clr)
{
    const draw_hijack dh { *this, clr };
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderDrawRectF(this->ptr(), SDL::FRect(area).addr()) == 0, ::SDL_GetError());
}

void renderer::fill_rect(const SDL::FRect& area, color clr)
{
    const draw_hijack dh { *this, clr };
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderFillRectF(this->ptr(), area.addr()) == 0, ::SDL_GetError());
}

void renderer::fill_target(color clr)
{
    const draw_hijack dh { *this, clr };
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderFillRectF(this->ptr(), nullptr) == 0, ::SDL_GetError());
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

color renderer::get_color() const
{
    color ret;

    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRenderDrawColor(this->ptr(), &ret.r, &ret.g, &ret.b, &ret.a) == 0, ::SDL_GetError());

    return ret;
}

void renderer::set_color(color clr)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawColor(this->ptr(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}

void renderer::internal_set_target(SDL_Texture* target)
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(this->ptr(), target) == 0, ::SDL_GetError());
}