#pragma once

#include <SDL2/SDL_render.h>
#include <halcyon/types/point.hpp>

namespace hal
{
    struct anchor
    {
        enum pos : lyo::u8
        {
            none,
            center,
            top_left,
            top_right,
            bottom_left,
            bottom_right
        };

        template <lyo::arithmetic T>
        constexpr static point<T> resolve(pos anch, point<T> pos, point<T> size)
        {
            switch (anch)
            {
            case anchor::none:
            case anchor::top_left:
                return pos;

            case anchor::top_right:
                pos.x -= size.x;
                break;

            case anchor::bottom_left:
                pos.y -= size.y;
                break;

            case anchor::bottom_right:
                pos -= size;
                break;

            case anchor::center:
                pos -= size / 2.0;
                break;
            }

            return pos;
        }
    };

    enum class flip : lyo::u8
    {
        none = SDL_FLIP_NONE,
        x = SDL_FLIP_HORIZONTAL,
        y = SDL_FLIP_VERTICAL,
        both = SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL
    };
}