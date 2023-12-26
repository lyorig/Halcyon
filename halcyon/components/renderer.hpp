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

        // Might as well leave the pure bitmask parameter here.
        renderer(window& wnd, lyo::u32 flags, lyo::pass_key<window>);

        void present(lyo::pass_key<window>) const;
        void clear(lyo::pass_key<window>) const;

        void set_target(texture& tx) const;
        void reset_target() const;

        void set_fill(color clr) const;
        void set_scale(point<lyo::f64> scale);
        void set_integer_scaling(bool enable);

        pixel_size output_size() const;
    };
} // namespace hal