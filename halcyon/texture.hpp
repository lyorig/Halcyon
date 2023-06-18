#pragma once

#include <SDL2/SDL_render.h>

#include "components/surface.hpp"

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal
{
    class window;

    class texture : public sdl_object<SDL_Texture, ::SDL_DestroyTexture>
    {
      public:

        enum flip : lyo::u8
        {
            none = SDL_FLIP_NONE,
            x    = SDL_FLIP_HORIZONTAL,
            y    = SDL_FLIP_VERTICAL,
            both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
        };

        texture(const window& wnd) noexcept;

        texture(const window& wnd, const pixel_size& size) noexcept;

        texture(const window& wnd, surface image) noexcept;

        const pixel_size& size() const noexcept;
        lyo::u8           opacity() const noexcept;

        void set_opacity(lyo::u8 value) const noexcept;

        void draw(const coordinate& pos, double scale = 1.0, double angle = 0.0, flip flip = none) const noexcept;
        void draw(const coordinate& pos, const pixel_area& src, double scale = 1.0, double angle = 0.0, flip flip = none) const noexcept;

      private:

        pixel_size m_size;

        const window& m_window;

        void render_copy(const coordinate& pos, double scale, double angle, flip flip) const noexcept;
        void render_copy(const coordinate& pos, const pixel_area& src, double scale, double angle, flip flip) const noexcept;
    };
}  // namespace hal