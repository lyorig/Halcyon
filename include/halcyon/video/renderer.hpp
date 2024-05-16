#pragma once

#include <span>

#include <SDL_render.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/enum_bitset.hpp>
#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/scaler.hpp>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>

// video/renderer.hpp:
// A proxy for creating and rendering textures etc. - more info below.

namespace hal
{
    class surface;

    namespace detail
    {
        class texture_base;
    }

    // Forward declarations for parameters and return types.
    class window;
    class copyer;

    class texture;
    class target_texture;

    enum class flip : u8
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    namespace info
    {
        class renderer;
    }

    extern template class detail::raii_object<SDL_Renderer, ::SDL_DestroyRenderer>;

    // A wrapper of SDL_Renderer. Essentially, this is the thing that does the rendering, and
    // is attached to a window. Multiple renderers can exist for a single window, i.e. a hardware-
    // accelerated one, plus a software fallback in case the former isn't available.
    // By default, renderers use hardware acceleration. You can override this via renderer flags.
    class renderer : public detail::raii_object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum class flags : u8
        {
            none           = 0,
            software       = SDL_RENDERER_SOFTWARE,
            accelerated    = SDL_RENDERER_ACCELERATED,
            vsync          = SDL_RENDERER_PRESENTVSYNC,
            target_texture = SDL_RENDERER_TARGETTEXTURE
        };

        renderer() = default;

        renderer(const hal::window& wnd, std::initializer_list<flags> f, pass_key<window>);

        // Clear (fill) the render target with the current draw color.
        void clear();

        // Present the back-buffer and clear it.
        void present();

        void point(const sdl::coord_point& pt);
        void line(const sdl::coord_point& from, const sdl::coord_point& to);
        void rect(const sdl::coord_rect& area);

        void fill(const sdl::coord_rect& area);
        void fill(const std::span<const sdl::coord_rect>& areas);
        void fill();

        // Get/set the rendering target.
        void target(target_texture& tx);
        void reset_target();

        // Get/set the color with which line/rect/fill drawing operations happen.
        hal::color color() const;
        void       color(hal::color clr);

        // Get/set the way blending happens with line/rect/fill operations.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        // Get/set the size of the "drawing board."
        pixel_point size() const;
        void        size(pixel_point sz);
        void        size(scaler scl);

        info::renderer info() const;

        // Texture creation functions.
        [[nodiscard]] texture        make_texture(const surface& surf) &;
        [[nodiscard]] target_texture make_target_texture(pixel_point size) &;

        // Create a copyer.
        [[nodiscard]] copyer draw(const detail::texture_base& tex);

    private:
        // Helper for setting the render target.
        void internal_target(SDL_Texture* target);
    };

    namespace info
    {
        class renderer : SDL_RendererInfo
        {
        public:
            using flag_bitset = detail::enum_bitset<hal::renderer::flags, decltype(SDL_RendererInfo::flags)>;

            renderer(const hal::renderer& rnd, pass_key<hal::renderer>);

            std::string_view name() const;

            flag_bitset flags() const;

            pixel_point max_texture_size() const;
        };
    }

    class copyer : public hal::detail::drawer<detail::texture_base, sdl::coord_t, renderer, copyer>
    {
    public:
        using drawer::drawer;

        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] copyer& rotate(f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] copyer& flip(enum flip f);

        // Outlines the current destination with the renderer's draw color.
        // Can be called at any time, but you probably want to call this
        // after properly setting the destination rectangle.
        [[nodiscard]] copyer& outline();

        void operator()();

    private:
        f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };
}