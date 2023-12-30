#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/components/surface.hpp>
#include <halcyon/internal/drawer.hpp>

// texture.cpp:
// A proper texture that can be drawn to a window (or another texture).

namespace hal
{
    class window;
    class texture;

    class draw final : public drawer<texture, SDL::coord_type, draw>
    {
    public:
        using drawer::drawer;

        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] draw& rotate(lyo::f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] draw& flip(enum flip f);

        void operator()() const;

    private:
        lyo::f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };

    class texture : public sdl_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    public:
        texture(window& wnd);
        texture(window& wnd, const pixel_size& size, SDL_TextureAccess access);
        texture(window& wnd, const surface& image);

        pixel_size   size() const;
        color::value opacity() const;

        const window& window() const;

        void set_opacity(color::value value);
        void set_color_mod(hal::color mod);

        // View-width and view-height helpers.
        pixel_size vw(lyo::f64 percent) const;
        pixel_size vh(lyo::f64 percent) const;

    protected:
        class window& m_window;

        // Multiple things can fail here, so it's better to have it as a separate function.
        static SDL_Texture* create_texture(class window& wnd, const pixel_size& sz, SDL_TextureAccess access);

    private:
        pixel_size internal_size() const;

        void query(Uint32* format, int* access, int* w, int* h) const;
    };

    // A texture that isn't drawn onto. It can change, but
    class static_texture : public texture
    {
    public:
        static_texture(class window& wnd);
        static_texture(class window& wnd, const surface& image);

        static_texture& operator=(const surface& surf);
    };

    class target_texture : public texture
    {
    public:
        target_texture(class window& wnd);
        target_texture(class window& wnd, const pixel_size& size);

        void resize(const pixel_size& sz);
    };
} // namespace hal