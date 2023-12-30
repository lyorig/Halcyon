#pragma once

#include <halcyon/components/surface.hpp>

// texture.cpp:
// Proper textures that can be drawn to a window (or another texture).

namespace hal
{
    class window;

    class texture_base : public sdl_object<SDL_Texture, &::SDL_DestroyTexture>
    {
    public:
        pixel_size   size() const;
        color::value opacity() const;

        void set_opacity(color::value value);
        void set_color_mod(hal::color mod);

    protected:
        texture_base() = default;
        texture_base(SDL_Texture* ptr);

    private:
        pixel_size internal_size() const;

        void query(Uint32* format, int* access, int* w, int* h) const;
    };

    // A texture that cannot be drawn onto, only reassigned.
    class texture : public texture_base
    {
    public:
        texture() = default;
        texture(window& wnd, const surface& surf);

        texture& change(window& wnd, const surface& surf);

    private:
        // Convenience function.
        static SDL_Texture* create(window& wnd, const surface& image);
    };

    // A texture that can be drawn onto.
    class target_texture : public texture_base
    {
    public:
        target_texture() = default;
        target_texture(window& wnd, const pixel_size& size);

        void resize(window& wnd, const pixel_size& sz);

    private:
        // Multiple things can fail here on top of it being a convenience function.
        static SDL_Texture* create(window& wnd, const pixel_size& sz);
    };
} // namespace hal