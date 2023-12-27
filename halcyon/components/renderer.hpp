#pragma once

#include <SDL2/SDL_render.h>

#include <halcyon/types/color.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/pass_key.hpp>

#include <halcyon/components/sdl_object.hpp>

namespace hal
{
    class window;
    class texture;

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

        class draw_hijack
        {
        public:
            draw_hijack(renderer& rnd, color new_clr)
                : m_rnd { rnd }
                , m_old { rnd.get_color() }
            {
                rnd.set_color(new_clr);
            }

            ~draw_hijack()
            {
                m_rnd.set_color(m_old);
            }

        private:
            renderer&   m_rnd;
            const color m_old;
        };

        // Might as well leave the pure bitmask parameter here.
        renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>);

        void present(lyo::pass_key<window>) const;
        void clear(lyo::pass_key<window>) const;

        void draw_line(const coord& from, const coord& to, color clr = color::white);
        void draw_rect(const coord_area& area, color clr = color::white);

        void set_target(texture& tx);
        void reset_target();

        color get_color() const;
        void  set_color(color clr);

        pixel_size output_size() const;

    private:
        void internal_set_target(SDL_Texture* target);
    };
}