#pragma once

#include <SDL2/SDL_render.h>

#include "components/surface.hpp"
#include "internal/config.hpp"

#include <optional>

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal {
enum class anchor : lyo::u8 {
    none,
    center,
    top_left,
    top_right,
    bottom_left,
    bottom_right
};

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

    // TODO: Anchor functionality...?
    class drawer {
    public:
        [[nodiscard]] drawer(const hal::texture& src);

        [[nodiscard]] drawer& to(const coordinate& pos);
        [[nodiscard]] drawer& to(const world_area& area);

        [[nodiscard]] drawer& from(const pixel_area& src);

        // Call this after setting the destination!
        [[nodiscard]] drawer& scale(lyo::f64 mul);

        [[nodiscard]] drawer& rotate(lyo::f64 angle);

        [[nodiscard]] drawer& flip(enum flip f);

        [[nodiscard]] drawer& anchor(enum anchor anch);

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