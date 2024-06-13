#pragma once

#include <span>

#include <SDL_render.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/raii_object.hpp>
#include <halcyon/internal/scaler.hpp>
#include <halcyon/utility/enum_bitset.hpp>

#include <halcyon/types/color.hpp>

#include <halcyon/utility/buffer.hpp>

#include <halcyon/video/types.hpp>

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

    namespace info::sdl
    {
        class renderer;
    }

    namespace detail
    {
        template <>
        class view_impl<SDL_Renderer> : public view_base<SDL_Renderer>
        {
        public:
            using view_base::view_base;

            hal::color color() const;

            blend_mode blend() const;

            pixel::point size() const;

            pixel::format pixel_format() const;

            info::sdl::renderer info() const;
        };
    }

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

        using flag_bitset = enum_bitset<flags, decltype(SDL_RendererInfo::flags)>;

        renderer() = default;

        renderer(const hal::window& wnd, std::initializer_list<flags> f);

        // Clear (fill) the render target with the current draw color.
        void clear();

        // Present the back-buffer and clear it.
        void present();

        // Draw a single point (pixel) with the current color.
        void draw(coord::point pt);

        // Draw a line with the current color.
        void draw(coord::point from, coord::point to);

        // Outline a rectangle with the current color.
        void draw(coord::rect area);

        void fill(coord::rect area);
        void fill(std::span<const coord::rect> areas);
        void fill();

        // Get/set the rendering target.
        void target(target_texture& tx);
        void reset_target();

        using view_impl::color;
        void color(hal::color clr);

        using view_impl::blend;
        void blend(blend_mode bm);

        using view_impl::size;
        void size(pixel::point sz);
        void size(scaler scl);

        view::window window();

        // Texture creation functions.
        [[nodiscard]] texture        make_texture(const surface& surf) &;
        [[nodiscard]] target_texture make_target_texture(pixel::point size) &;

        // Render a texture via a builder.
        [[nodiscard]] copyer render(const detail::texture_base& tex);

    private:
        // Helper for setting the render target.
        void internal_target(SDL_Texture* target);
    };

    namespace info
    {
        class renderer;

        namespace sdl
        {
            class renderer : SDL_RendererInfo
            {
            public:
                renderer() = default;

                renderer(const view::renderer& rnd, pass_key<view::renderer>);

                std::string_view name() const;

                hal::renderer::flag_bitset flags() const;

                std::span<const pixel::format> formats() const;

                pixel::point max_texture_size() const;

                SDL_RendererInfo* get();

                friend std::ostream& operator<<(std::ostream& str, const info::sdl::renderer& inf);
            };
        }

        class renderer
        {
        public:
            renderer() = default;

            // Compress native renderer info.
            renderer(const sdl::renderer& src);

            std::string_view name;
            pixel::point     max_texture_size;

            hal::renderer::flag_bitset flags;
            buffer<pixel::format>      formats;
        };
    }

    class copyer : public hal::detail::drawer<detail::texture_base, coord_t, renderer, copyer>
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

        // Finish the operation.
        void operator()();

    private:
        f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };
}