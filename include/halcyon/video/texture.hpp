#pragma once

#include <SDL_render.h>

#include <halcyon/utility/pass_key.hpp>

#include <halcyon/internal/raii_object.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/video/types.hpp>

// video/texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    class surface;

    namespace detail
    {
        template <>
        class view_impl<SDL_Texture> : public view_base<SDL_Texture>
        {
        public:
            using view_base::view_base;

            pixel::point size() const;

            color::value_t opacity() const;

            color::value_t alpha_mod() const;

            color color_mod() const;

            blend_mode blend() const;

            pixel::format pixel_format() const;

        private:
            void query(Uint32* format, int* access, int* w, int* h) const;
        };

        // Common texture functionality.
        class texture_base : public detail::raii_object<SDL_Texture, &::SDL_DestroyTexture>
        {
        public:
            using view_impl::opacity;
            void opacity(color::value_t value);

            using view_impl::alpha_mod;
            void alpha_mod(color::value_t val);

            using view_impl::color_mod;
            void color_mod(color mod);

            using view_impl::blend;
            void blend(blend_mode bm);

        protected:
            texture_base() = default;

            texture_base(SDL_Texture* ptr);
        };
    }

    // Forward declarations for parameters and return types.
    class renderer;

    // A texture that cannot be drawn onto, only reassigned.
    class texture : public detail::texture_base
    {
    public:
        texture() = default;

        // [private] Textures are created with renderer::load().
        texture(renderer& rnd, const surface& surf);
    };

    // A texture that can be drawn onto.
    class target_texture : public detail::texture_base
    {
    public:
        target_texture() = default;

        // [private] Target textures are created with renderer::load().
        target_texture(renderer& rnd, pixel::format fmt, pixel::point size);
    };
}