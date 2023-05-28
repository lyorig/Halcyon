#include "renderer.hpp"

#include <halcyon/window.hpp>

using namespace halcyon;

renderer::renderer(window& window, lyo::u32 flags) noexcept :
    sdl_object { ::SDL_CreateRenderer(window, -1, flags), "Couldn't create renderer" }
{
}

void renderer::present() const noexcept
{
    ::SDL_RenderPresent(m_object);
}

void renderer::clear() const noexcept
{
    HALCYON_VERIFY(::SDL_RenderClear(m_object) == 0, "SDL_RenderClear failed");
}

pixel_size renderer::output_size() const noexcept
{
    point<int> size;
    HALCYON_VERIFY(::SDL_GetRendererOutputSize(m_object, &size.x, &size.y) == 0, "Couldn't get renderer output size");
    return size;
}

void renderer::set_target(texture& tx) const noexcept
{
    HALCYON_VERIFY(::SDL_SetRenderTarget(m_object, tx) == 0, "Couldn't set render target");
}

void renderer::reset_target() const noexcept
{
    HALCYON_VERIFY(::SDL_SetRenderTarget(m_object, NULL) == 0, "Couldn't set render target");
}
