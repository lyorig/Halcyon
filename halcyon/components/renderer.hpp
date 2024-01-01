#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/enums/blend.hpp>
#include <halcyon/internal/drawer.hpp>
#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class draw;

    class window;
    class renderer;

    class texture_base;
    class target_texture;

    class target_lock;
    class color_lock;

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

        void draw_line(const coord& from, const coord& to);
        void draw_rect(const coord_area& area);

        void fill_rect(const SDL::FRect& area);
        void fill_target();

        void set_target(target_texture& tx);
        void reset_target();

        color get_color() const;
        void  set_color(color clr);

        blend_mode get_blend() const;
        void       set_blend(blend_mode bm);

        target_lock lock_target(target_texture& tx);
        color_lock  lock_color(color clr);

        pixel_size output_size() const;

        void internal_render_copy(const texture_base& tex, const SDL_Rect* src, const SDL_FRect* dst, lyo::f64 angle, flip f, lyo::pass_key<draw>);

    private:
        void internal_set_target(SDL_Texture* target);
    };

    class color_lock
    {
    public:
        color_lock(renderer& rnd, color new_clr, lyo::pass_key<renderer>);

        ~color_lock();

    private:
        renderer&   m_rnd;
        const color m_old;
    };

    class target_lock
    {
    public:
        target_lock(renderer& rnd, target_texture& tgt, lyo::pass_key<renderer>);

        ~target_lock();

    private:
        renderer& m_rnd;
    };
}