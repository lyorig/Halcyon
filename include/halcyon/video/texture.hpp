#pragma once

#include <halcyon/internal/sdl_object.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>

#include <SDL_render.h>

// video/texture.cpp:
// Proper textures that can be drawn to a window (or a target texture).

namespace hal
{
    // Forward declarations for parameters and return types.
    class renderer;
    class copyer;
    class surface;

    namespace detail
    {
        // Common texture functionality.
        class texture_base : public sdl::object<SDL_Texture, &::SDL_DestroyTexture>
        {
        public:
            pixel_point size() const;

            color::value_t opacity() const;
            void           opacity(color::value_t value);

            color color_mod() const;
            void  color_mod(hal::color mod);

            blend_mode blend() const;
            void       blend(blend_mode bm);

        protected:
            texture_base() = default;
            texture_base(SDL_Texture* ptr);

            void reset(SDL_Texture* ptr);

        private:
            void query(std::uint32_t* format, int* access, int* w, int* h) const;
        };
    }

    // A texture that cannot be drawn onto, only reassigned.
    class texture : public detail::texture_base
    {
    public:
        texture() = default;
        texture(renderer& rnd, const surface& surf);

    private:
        // Convenience function.
        static SDL_Texture* create(renderer& rnd, const surface& image);
    };

    // A texture that can be drawn onto.
    class target_texture : public detail::texture_base
    {
    public:
        target_texture() = default;
        target_texture(renderer& rnd, const pixel_point& size);

    private:
        // Multiple things can fail here on top of it being a convenience function.
        static SDL_Texture* create(renderer& rnd, const pixel_point& sz);
    };
} // namespace hal