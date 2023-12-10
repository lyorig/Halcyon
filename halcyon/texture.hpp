#pragma once

#include <SDL2/SDL_render.h>

#include "components/surface.hpp"

#include <optional>

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal {
struct anchor {
    enum pos : lyo::u8 {
        none,
        center,
        top_left,
        top_right,
        bottom_left,
        bottom_right
    };

    static coordinate resolve(pos anch, world_area from)
    {
        switch (anch) {
        case anchor::none:
        case anchor::top_left:
            return from.pos;

        case anchor::top_right:
            from.pos.x -= from.size.x;
            break;

        case anchor::bottom_left:
            from.pos.y -= from.size.y;
            break;

        case anchor::bottom_right:
            from.pos -= from.size;
            break;

        case anchor::center:
            from.pos -= from.size / 2;
            break;
        }

        return from.pos;
    }
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

    class draw {
    public:
        [[nodiscard]] draw(const hal::texture& src);

        [[nodiscard]] draw& to(const coordinate& pos);
        [[nodiscard]] draw& to(const world_area& area);

        [[nodiscard]] draw& from(const pixel_area& src);

        // Call this after setting the destination!
        [[nodiscard]] draw& scale(lyo::f64 mul);

        [[nodiscard]] draw& rotate(lyo::f64 angle);

        [[nodiscard]] draw& flip(enum flip f);

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