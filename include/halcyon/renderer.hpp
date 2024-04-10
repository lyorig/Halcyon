#pragma once

#include <SDL_render.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/internal/sdl_object.hpp>
#include <halcyon/other/blend.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <span>

// renderer.hpp:
// A wrapper of SDL_Renderer. Essentially, this is the thing that does the rendering, and
// is attached to a window. Multiple renderers can exist for a single window, i.e. a hardware-
// accelerated one, plus a software fallback in case the former isn't available.

namespace hal
{
    // Forward declarations for parameters and return types.
    class window;
    class copyer;

    namespace detail
    {
        class texture_base;
    }

    class target_texture;

    enum class flip : lyo::u8
    {
        none = SDL_FLIP_NONE,
        x    = SDL_FLIP_HORIZONTAL,
        y    = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };

    class renderer : public sdl::object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum class flags : lyo::u8
        {
            none        = 0,
            software    = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync       = SDL_RENDERER_PRESENTVSYNC
        };

        renderer(window& wnd, std::initializer_list<flags> flags);

        // Present the back-buffer and clear it.
        void present();

        void draw_point(const sdl::coord_point& pt);
        void draw_line(const sdl::coord_point& from, const sdl::coord_point& to);
        void draw_rect(const sdl::coord_rect& area);

        void fill_rect(const sdl::coord_rect& area);
        void fill_rects(const std::span<const sdl::coord_rect>& areas);
        void fill_target();

        // Get/set the rendering target.
        void target(target_texture& tx);
        void retarget();

        // Get/set the color with which line/rect/fill drawing operations happen.
        color draw_color() const;
        void  draw_color(color clr);

        // Get/set the way blending happens with line/rect/fill operations.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        // Get/set the size of the "drawing board."
        pixel_point size() const;
        void        size(const pixel_point& sz);

        // Create a copyer.
        [[nodiscard]] copyer draw(const detail::texture_base& tex);

        // Public, but only accessible to the draw class.
        void internal_render_copy(const detail::texture_base& tex, const sdl::pixel_rect* src, const sdl::coord_rect* dst, lyo::f64 angle, flip f, lyo::pass_key<copyer>);

    private:
        void clear();

        // Helper for setting the render target.
        void internal_target(SDL_Texture* target);
    };

    class copyer : public detail::drawer<detail::texture_base, sdl::coord_t, renderer, copyer>
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

    // C-tor: Sets the renderer's draw color.
    // D-tor: Sets the draw color back to the previous one.
    class color_lock
    {
    public:
        color_lock(renderer& rnd);
        color_lock(renderer& rnd, color new_clr);
        ~color_lock();

        void set(color clr);

    private:
        renderer&   m_rnd;
        const color m_old;
    };

    // C-tor: Sets the renderer's target texture.
    // D-tor: Sets the target back to the renderer's window.
    class target_lock
    {
    public:
        target_lock(renderer& rnd, target_texture& tgt);
        ~target_lock();

        void set(target_texture& tgt);

    private:
        renderer& m_rnd;
    };
}