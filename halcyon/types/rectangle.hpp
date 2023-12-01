#pragma once

#include <SDL2/SDL_rect.h>

#include "point.hpp"
#include "tags.hpp"

namespace hal {
template <lyo::arithmetic T>
struct rectangle {
    point<T> pos, size;

    constexpr rectangle() = default;

    constexpr rectangle(T x, T y, T w, T h)
        : pos { x, y }
        , size { w, h }
    {
    }

    constexpr rectangle(as_size_tag, const point<T>& size)
        : size { size }
    {
    }

    constexpr rectangle(const point<T>& pos, const point<T>& size)
        : pos { pos }
        , size { size }
    {
    }

    template <lyo::arithmetic Convert>
    constexpr explicit operator rectangle<Convert>() const
    {
        return rectangle<Convert> { static_cast<point<Convert>>(pos),
            static_cast<point<Convert>>(size) };
    }

    constexpr rectangle operator*(lyo::f64 mul) const
    {
        return rectangle { pos, size * mul };
    }

    constexpr rectangle operator/(lyo::f64 div) const
    {
        return rectangle { pos, size / div };
    }

    constexpr rectangle& operator*=(lyo::f64 mul)
    {
        size *= mul;

        return *this;
    }

    constexpr rectangle& operator/=(lyo::f64 div)
    {
        size /= div;

        return *this;
    }

    // "Intersects with" operator.
    constexpr bool operator|(const rectangle& other) const
    {
        return pos.x + size.x >= other.pos.x && pos.x <= other.pos.x + other.size.x && pos.y <= other.pos.y + other.size.y && pos.y + size.y >= other.pos.y;
    }

    constexpr operator SDL_Rect() const
    {
        using type = decltype(SDL_Rect::x);

        return SDL_Rect {
            lyo::round_cast<type>(pos.x),
            lyo::round_cast<type>(pos.y),
            lyo::round_cast<type>(size.x),
            lyo::round_cast<type>(size.y),
        };
    }

    constexpr operator SDL_FRect() const
    {
        using type = decltype(SDL_FRect::x);

        return SDL_FRect {
            lyo::round_cast<type>(pos.x),
            lyo::round_cast<type>(pos.y),
            lyo::round_cast<type>(size.x),
            lyo::round_cast<type>(size.y),
        };
    }
};
} // namespace hal