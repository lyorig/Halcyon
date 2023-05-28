#pragma once

#include <SDL2/SDL_render.h>

#include <optional>

#include "sdl_object.hpp"
#include <halcyon/types.hpp>

namespace halcyon
{
    class window;
    class texture;

    class renderer : public sdl_object<SDL_Renderer, ::SDL_DestroyRenderer>
    {
        friend class window;

      public:

        enum flag_t
        {
            software    = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync       = SDL_RENDERER_PRESENTVSYNC
        };

        void set_target(texture& tx) const noexcept;
        void reset_target() const noexcept;

      private:

        renderer(window& window, lyo::u32 flags) noexcept;

        void present() const noexcept;
        void clear() const noexcept;

        pixel_size output_size() const noexcept;
    };
}  // namespace halcyon