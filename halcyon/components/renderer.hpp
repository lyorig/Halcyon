#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include "sdl_object.hpp"

namespace hal
{
    class window;
    class texture;

    class renderer : public sdl_object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
      public:

        enum flag : lyo::u8
        {
            none        = 0,
            software    = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync       = SDL_RENDERER_PRESENTVSYNC
        };

        renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>) noexcept;

        void present(lyo::pass_key<window>) const noexcept;
        void clear(lyo::pass_key<window>) const noexcept;

        void set_target(texture& tx) const noexcept;
        void reset_target() const noexcept;

        void set_fill(rgba color) const noexcept;

        pixel_size output_size() const noexcept;
    };
}  // namespace hal