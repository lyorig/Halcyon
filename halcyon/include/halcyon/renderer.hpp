#pragma once

#include <SDL_render.h>

#include <halcyon/enums/anchor.hpp>
#include <halcyon/enums/blend.hpp>
#include <halcyon/other/drawer.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>
#include <span>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class window;

    class texture_base;
    class target_texture;

    class copyer;

    class renderer : public sdl::object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum flags : lyo::u8
        {
            none        = 0,
            software    = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync       = SDL_RENDERER_PRESENTVSYNC
        };

        renderer(window& wnd, std::initializer_list<flags> flags);

        void present();

        void draw_line(const sdl::coord_point& from, const sdl::coord_point& to);
        void draw_rect(const sdl::coord_rect& area);

        void fill_rect(const sdl::coord_rect& area);
        void fill_rects(const std::span<const sdl::coord_rect>& areas);
        void fill_target();

        void target(target_texture& tx);
        void target();

        // Get the color with which line/rect/fill drawing operations happen.
        color draw_color() const;
        void  draw_color(color clr);

        // Set the way blending happens with line/rect/fill operations.
        blend_mode blend() const;
        void       blend(blend_mode bm);

        // Get the size of the "drawing board."
        pixel_point size() const;
        void        size(const pixel_point& sz);

        [[nodiscard]] copyer draw(const texture_base& tex);

        // Public, but only accessible to the draw class.
        void internal_render_copy(const texture_base& tex, const sdl::pixel_rect* src, const sdl::coord_rect* dst, lyo::f64 angle, flip f, lyo::pass_key<copyer>);

    private:
        void clear();

        void internal_target(SDL_Texture* target);
    };

    class copyer : public drawer<texture_base, sdl::coord_t, renderer, copyer>
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