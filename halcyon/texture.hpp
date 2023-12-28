#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/components/surface.hpp>
#include <halcyon/internal/drawer.hpp>

// texture.cpp:
// A proper texture that can be drawn to a window (or another texture).

namespace hal
{
    class window;

    class texture : public sdl_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    public:
        texture(window& wnd);
        texture(window& wnd, const pixel_size& size);
        texture(window& wnd, const surface& image);

        pixel_size size() const;
        lyo::u8    opacity() const;

        const window& window() const;

        void set_opacity(color::value value);
        void set_color_mod(hal::color mod);
        void set_as_target();

        // View-width and view-height helpers.
        pixel_size vw(lyo::f64 percent) const;
        pixel_size vh(lyo::f64 percent) const;

        texture& operator=(const surface& image);

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

    private:
        class window& m_window;

        pixel_size internal_size() const;

        SDL_TextureAccess get_access() const;

        void query(Uint32* format, int* access, int* w, int* h) const;
    };
} // namespace hal