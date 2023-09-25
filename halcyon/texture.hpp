#pragma once

#include <SDL2/SDL_render.h>

#include "components/surface.hpp"
#include "internal/config.hpp"

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal
{
    class window;

    enum class anchor : lyo::u8
    {
        center,
        top_left,
        top_right,
        bottom_left,
        bottom_right
    };

    enum class flip : lyo::u8
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    class texture : public sdl_object<SDL_Texture, &::SDL_DestroyTexture>
    {
      public:

        texture(const window& wnd) noexcept;
        texture(const window& wnd, const pixel_size& size) noexcept;
        texture(const window& wnd, surface image) noexcept;

        const pixel_size& size() const noexcept;
        lyo::u8           opacity() const noexcept;

        void set_opacity(lyo::u8 value) const noexcept;

        // Position (+ source).
        void draw(const coordinate& pos, lyo::f64 scale = 1.0, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;
        void draw(const coordinate& pos, const pixel_size& size, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;
        void draw(const coordinate& pos, const pixel_area& src, lyo::f64 scale = 1.0, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;

        // Destination (+ source).
        void draw(const world_area& dest, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;
        void draw(const world_area& dest, const pixel_area& src, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;

        // Anchor variants.
        void draw(anchor anch, lyo::f64 scale = 1.0, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;
        void draw(anchor anch, const pixel_size& size, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;
        void draw(const coordinate& pos, anchor anch, lyo::f64 angle = 0.0, flip f = flip::none) const noexcept;

        void set_as_target() noexcept;

        // View-width and view-height helpers.
        pixel_size vw(lyo::f64 percent) const noexcept;
        pixel_size vh(lyo::f64 percent) const noexcept;

        texture& operator=(surface image) noexcept;

      private:

        using dest_rect = std::conditional_t<cfg::subpixel_drawing_precision, SDL_FRect, SDL_Rect>;

        void render_copy(const world_area& dst, lyo::f64 angle, flip f) const noexcept;
        void render_copy(const world_area& dst, const pixel_area& src, lyo::f64 angle, flip f) const noexcept;

        constexpr coordinate resolve_anchor(anchor anch, const coordinate& pos, const pixel_size& size) const noexcept;
        constexpr coordinate resolve_anchor(anchor anch, const world_area& dest, const pixel_size& size) const noexcept;

        pixel_size internal_size() const noexcept;

        pixel_size m_size;

        const window& m_window;
    };
}  // namespace hal