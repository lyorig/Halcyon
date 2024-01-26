#pragma once

#include <halcyon/components/surface.hpp>

// texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    class renderer;
    class copyer;

    class texture_base : public sdl::object<SDL_Texture, &::SDL_DestroyTexture>
    {
    public:
        pixel_point size() const;

        color::value opacity() const;
        void         set_opacity(color::value value);

        color color_mod() const;
        void  set_color_mod(hal::color mod);

        blend_mode blend() const;
        void       set_blend(blend_mode bm);

    protected:
        texture_base() = default;
        texture_base(SDL_Texture* ptr);

        void reset(SDL_Texture* ptr);

    private:
        pixel_point internal_size() const;

        void query(Uint32* format, int* access, int* w, int* h) const;
    };

    // A texture that cannot be drawn onto, only reassigned.
    class texture : public texture_base
    {
    public:
        texture() = default;
        texture(renderer& rnd, const surface& surf);

        texture& change(renderer& rnd, const surface& surf);

    private:
        // Convenience function.
        static SDL_Texture* create(renderer& rnd, const surface& image);
    };

    // A texture that can be drawn onto.
    class target_texture : public texture_base
    {
    public:
        target_texture() = default;
        target_texture(renderer& rnd, const pixel_point& size);

        void resize(renderer& rnd, pixel_point sz);

    private:
        // Multiple things can fail here on top of it being a convenience function.
        static SDL_Texture* create(renderer& rnd, const pixel_point& sz);
    };

    class copyer final : public drawer<texture_base, sdl::coord_t, renderer, copyer>
    {
    public:
        using drawer::drawer;

        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] copyer& rotate(lyo::f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] copyer& flip(enum flip f);

        void operator()();

    private:
        lyo::f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };
} // namespace hal