#include "renderer.hpp"

#include <halcyon/window.hpp>

using namespace hal;

renderer::renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>)
    : sdl_object { ::SDL_CreateRenderer(wnd.ptr(), -1, flags) }
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(m_object.get(), SDL_BLENDMODE_BLEND) == 0, ::SDL_GetError());
}

void renderer::present(lyo::pass_key<window>) const
{
    ::SDL_RenderPresent(m_object.get());
}

void renderer::clear(lyo::pass_key<window>) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderClear(m_object.get()) == 0, ::SDL_GetError());
}

pixel_size renderer::output_size() const
{
    point<int> size;
    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRendererOutputSize(m_object.get(), &size.x, &size.y) == 0, ::SDL_GetError());
    return static_cast<pixel_size>(size);
}

void renderer::set_target(texture& tx) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(m_object.get(), tx.ptr()) == 0, ::SDL_GetError());
}

void renderer::reset_target() const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(m_object.get(), nullptr) == 0, ::SDL_GetError());
}

void renderer::set_fill(color clr) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawColor(m_object.get(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}
