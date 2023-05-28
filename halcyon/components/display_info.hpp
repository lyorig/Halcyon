#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/types.hpp>

namespace halcyon
{
    class engine;

    struct display_info
    {
        friend class engine;

        pixel_size size;

        lyo::u16 hz;

        lyo::u8 bpp;

      private:

        display_info(const SDL_DisplayMode& dm) noexcept;
    };
}  // namespace halcyon