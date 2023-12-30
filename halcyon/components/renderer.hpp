#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/internal/drawer.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class window;
    class texture_base;
    class renderer;
    class target_texture;

    class draw final : public drawer<texture_base, SDL::coord_type, draw>
    {
    public:
        using drawer::drawer;
        // Set the texture's rotation.
        // Can be called at any time.
        [[nodiscard]] draw& rotate(lyo::f64 angle);

        // Set the texture's flip.
        // Can be called at any time.
        [[nodiscard]] draw& flip(enum flip f);

        void operator()(window& wnd) const;

    private:
        lyo::f64 m_angle { 0.0 };

        enum flip m_flip
        {
            flip::none
        };
    };

    class draw_hijack
    {
    public:
        draw_hijack(window& rnd, color new_clr);
        draw_hijack(renderer& rnd, color new_clr);

        ~draw_hijack();

    private:
        renderer&   m_rnd;
        const color m_old;
    };

    class target_hijack
    {
    public:
        target_hijack(window& wnd, target_texture& tgt);
        target_hijack(renderer& rnd, target_texture& tgt);

        ~target_hijack();

    private:
        renderer& m_rnd;
    };

    class renderer : public sdl_object<SDL_Renderer, &::SDL_DestroyRenderer>
    {
    public:
        enum flags : lyo::u8
        {
            none = 0,
            software = SDL_RENDERER_SOFTWARE,
            accelerated = SDL_RENDERER_ACCELERATED,
            vsync = SDL_RENDERER_PRESENTVSYNC
        };

        // Might as well leave the pure bitmask parameter here.
        renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>);

        void present(lyo::pass_key<window>) const;
        void clear(lyo::pass_key<window>) const;

        void draw_line(const coord& from, const coord& to, color clr);
        void draw_rect(const coord_area& area, color clr);

        void fill_rect(const SDL::FRect& area, color clr);
        void fill_target(color clr);

        void set_target(target_texture& tx);
        void reset_target();

        color get_color() const;
        void  set_color(color clr);

        pixel_size output_size() const;

    private:
        void internal_set_target(SDL_Texture* target);
    };
}