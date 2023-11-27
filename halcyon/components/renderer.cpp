#include "renderer.hpp"

#include <halcyon/window.hpp>

using namespace hal;

renderer::renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>) noexcept
    :
    sdl_object { ::SDL_CreateRenderer(wnd.ptr(), -1, flags) }
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderDrawBlendMode(m_object.get(), SDL_BLENDMODE_BLEND) == 0, ::SDL_GetError());
}

void renderer::present(lyo::pass_key<window>) const noexcept
{
    ::SDL_RenderPresent(m_object.get());
}

void renderer::clear(lyo::pass_key<window>) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_RenderClear(m_object.get()) == 0, ::SDL_GetError());
}

pixel_size renderer::output_size() const noexcept
{
    point<int> size;
    HAL_DEBUG_ASSERT(::SDL_GetRendererOutputSize(m_object.get(), &size.x, &size.y) == 0, ::SDL_GetError());
    return static_cast<pixel_size>(size);
}

void renderer::set_target(texture& tx) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderTarget(m_object.get(), tx.ptr()) == 0, ::SDL_GetError());
}

void renderer::reset_target() const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderTarget(m_object.get(), NULL) == 0, ::SDL_GetError());
}

void renderer::set_fill(color clr) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderDrawColor(m_object.get(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}
