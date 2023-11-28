#pragma once

#include <SDL2/SDL_rect.h>

#include "point.hpp"
#include "tags.hpp"

namespace hal
{
    template <lyo::arithmetic T>
    struct rectangle
    {
        point<T> pos, size;

        constexpr rectangle() noexcept = default;

        constexpr rectangle(T x, T y, T w, T h) noexcept :
            pos { x, y },
            size { w, h }
        {
        }

        constexpr rectangle(as_size_tag, const point<T>& size) noexcept
            :
            size { size }
        {
        }

        constexpr rectangle(const point<T>& pos, const point<T>& size) noexcept
            :
            pos { pos },
            size { size }
        {
        }

        template <lyo::arithmetic Convert>
        constexpr explicit operator rectangle<Convert>() const noexcept
        {
            return rectangle<Convert> { static_cast<point<Convert>>(pos),
                static_cast<point<Convert>>(size) };
        }

        constexpr rectangle operator*(lyo::f64 mul) const noexcept
        {
            return rectangle { pos, size * mul };
        }

        constexpr rectangle operator/(lyo::f64 div) const noexcept
        {
            return rectangle { pos, size / div };
        }

        constexpr rectangle& operator*=(lyo::f64 mul) noexcept
        {
            size *= mul;

            return *this;
        }

        constexpr rectangle& operator/=(lyo::f64 div) noexcept
        {
            size /= div;

            return *this;
        }

        // "Intersects with" operator.
        constexpr bool operator|(const rectangle& other) const noexcept
        {
            return pos.x + size.x >= other.pos.x &&
                   pos.x <= other.pos.x + other.size.x &&
                   pos.y <= other.pos.y + other.size.y &&
                   pos.y + size.y >= other.pos.y;
        }

        constexpr operator SDL_Rect() const noexcept
        {
            using type = decltype(SDL_Rect::x);

            return SDL_Rect {
                lyo::round_cast<type>(pos.x),
                lyo::round_cast<type>(pos.y),
                lyo::round_cast<type>(size.x),
                lyo::round_cast<type>(size.y),
            };
        }

        constexpr operator SDL_FRect() const noexcept
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
}  // namespace hal