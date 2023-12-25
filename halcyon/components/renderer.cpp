#include "renderer.hpp"

#include <halcyon/window.hpp>

using namespace hal;

renderer::renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>)
    : sdl_object { ::SDL_CreateRenderer(wnd.ptr(), -1, flags) }
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawBlendMode(this->ptr(), SDL_BLENDMODE_BLEND) == 0, ::SDL_GetError());
}

void renderer::present(lyo::pass_key<window>) const
{
    ::SDL_RenderPresent(this->ptr());
}

void renderer::clear(lyo::pass_key<window>) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_RenderClear(this->ptr()) == 0, ::SDL_GetError());
}

pixel_size renderer::output_size() const
{
    point<int> size;
    HAL_DEBUG_ASSERT_VITAL(::SDL_GetRendererOutputSize(this->ptr(), &size.x, &size.y) == 0, ::SDL_GetError());
    return pixel_size(size);
}

void renderer::set_target(texture& tx) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(this->ptr(), tx.ptr()) == 0, ::SDL_GetError());
}

void renderer::reset_target() const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderTarget(this->ptr(), nullptr) == 0, ::SDL_GetError());
}

void renderer::set_fill(color clr) const
{
    HAL_DEBUG_ASSERT_VITAL(::SDL_SetRenderDrawColor(this->ptr(), clr.r, clr.g, clr.b, clr.a) == 0, ::SDL_GetError());
}
