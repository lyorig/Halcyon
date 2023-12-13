#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/components/surface.hpp>
#include <halcyon/enums/anchor.hpp>

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal {

enum class flip : lyo::u8 {
    none = SDL_FLIP_NONE,
    x = SDL_FLIP_HORIZONTAL,
    y = SDL_FLIP_VERTICAL,
    both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
};

class window;

class texture : public sdl_object<SDL_Texture, &::SDL_DestroyTexture> {
public:
    texture(const window& wnd);
    texture(const window& wnd, const pixel_size& size);
    texture(const window& wnd, const surface& image);

    const pixel_size& size() const;
    lyo::u8 opacity() const;

    void set_opacity(lyo::u8 value) const;

    void set_as_target();

    // View-width and view-height helpers.
    pixel_size vw(lyo::f64 percent) const;
    pixel_size vh(lyo::f64 percent) const;

    texture& operator=(const surface& image);

    class draw {
    public:
        [[nodiscard]] draw(const hal::texture& src);

        // Set where to draw the texture.
        // Call before scaling and anchoring.
        [[nodiscard]] draw& to(const coordinate& pos);

        // Set the destination rectangle for the texture.
        // Call before scaling and anchoring.
        [[nodiscard]] draw& to(const world_area& area);

        // Stretch the texture across the board.
        // Do not use with scaling and anchoring.
        [[nodiscard]] draw& to(fill_tag);

        // Set the texture's source rectangle.
        // Can be called at any time.
        [[nodiscard]] draw& from(const pixel_area& src);

        // Set the texture's scale.
        // Call after setting the destination and before anchoring.
        [[nodiscard]] draw& scale(lyo::f64 mul);

        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] draw& rotate(lyo::f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] draw& flip(enum flip f);

        // Anchor the texture from the destination position.
        // Call after setting the destination and scaling.
        [[nodiscard]] draw& anchor(anchor::pos anch);

        void operator()() const;

    private:
        const texture& m_this;

        rect_wrap m_src; // The "x" field has a special value to signalize an unused rect.
        frect_wrap m_dst; // Initialized in c-tor.

        lyo::f64 m_angle { 0.0 };

        enum flip m_flip { flip::none };
    };

private:
    pixel_size internal_size() const;

    pixel_size m_size;

public:
    const class window& window;
};
} // namespace hal