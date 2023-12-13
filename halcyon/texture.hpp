#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/components/surface.hpp>
#include <halcyon/internal/drawer.hpp>

/* texture.cpp:
   A proper texture that can be drawn to a window.
   As SDL disallows texture sharing between different renderers,
   drawing is delegated to this class instead, with no way to change
   the window it's being rendered to. */

namespace hal
{

    class window;

    class texture : public sdl_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    public:
        texture(const window& wnd);
        texture(const window& wnd, const pixel_size& size);
        texture(const window& wnd, const surface& image);

        const pixel_size& size() const;
        lyo::u8           opacity() const;

        void set_opacity(lyo::u8 value) const;

        void set_as_target();

        // View-width and view-height helpers.
        pixel_size vw(lyo::f64 percent) const;
        pixel_size vh(lyo::f64 percent) const;

        texture& operator=(const surface& image);

        class draw final : public drawer<texture, SDL::position_type, draw>
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
        pixel_size internal_size() const;

        pixel_size m_size;

    public:
        const class window& window;
    };
} // namespace hal