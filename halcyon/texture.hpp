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
    texture(const window& wnd) noexcept;
    texture(const window& wnd, const pixel_size& size) noexcept;
    texture(const window& wnd, const surface& image) noexcept;

    const pixel_size& size() const noexcept;
    lyo::u8 opacity() const noexcept;

    void set_opacity(lyo::u8 value) const noexcept;

    void set_as_target() noexcept;

    // View-width and view-height helpers.
    pixel_size vw(lyo::f64 percent) const noexcept;
    pixel_size vh(lyo::f64 percent) const noexcept;

    texture& operator=(const surface& image) noexcept;

    class drawer {
    public:
        drawer(const hal::texture& src) noexcept;

        drawer& to(const coordinate& pos) noexcept;
        drawer& to(const world_area& area) noexcept;

        drawer& from(const pixel_area& src) noexcept;

        drawer& scale(lyo::f64 mul) noexcept;

        drawer& rotate(lyo::f64 angle) noexcept;

        void operator()() const noexcept;

    private:
        std::optional<SDL_Rect> m_src;

        hal::world_area m_dst;

        lyo::f64 m_scale { 1.0 };
        lyo::f64 m_angle { 0.0 };

        const texture& m_this;
    };

private:
    pixel_size
    internal_size() const noexcept;

    pixel_size m_size;

public:
    const class window& window;
};
} // namespace hal