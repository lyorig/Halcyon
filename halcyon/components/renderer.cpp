#include "renderer.hpp"

#include <halcyon/window.hpp>

using namespace hal;

renderer::renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>) noexcept :
    sdl_object { ::SDL_CreateRenderer(wnd.ptr(), -1, flags) }
{
}

void renderer::present(lyo::pass_key<window>) const noexcept
{
    ::SDL_RenderPresent(m_object);
}

void renderer::clear(lyo::pass_key<window>) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_RenderClear(m_object) == 0, ::SDL_GetError());
}

pixel_size renderer::output_size() const noexcept
{
    point<int> size;
    HAL_DEBUG_ASSERT(::SDL_GetRendererOutputSize(m_object, &size.x, &size.y) == 0, ::SDL_GetError());
    return size;
}

void renderer::set_target(texture& tx) const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderTarget(m_object, tx.ptr()) == 0, ::SDL_GetError());
}

void renderer::reset_target() const noexcept
{
    HAL_DEBUG_ASSERT(::SDL_SetRenderTarget(m_object, NULL) == 0, ::SDL_GetError());
}
