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

        constexpr rectangle(as_size_t, const point<T>& size) noexcept :
            size { size }
        {
        }

        constexpr rectangle(const point<T>& pos, const point<T>& size) noexcept :
            pos { pos },
            size { size }
        {
        }

        template <lyo::arithmetic Convert>
        constexpr explicit operator rectangle<Convert>() const noexcept
        {
            return rectangle<Convert> {
                static_cast<point<Convert>>(pos),
                static_cast<point<Convert>>(size)
            };
        }

        constexpr rectangle operator*(lyo::f64 mul) const noexcept
        {
            return rectangle {
                pos,
                size * mul
            };
        }

        constexpr rectangle operator/(lyo::f64 div) const noexcept
        {
            return rectangle {
                pos,
                size / div
            };
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
            const auto l1 { pos.x };
            const auto l2 { other.pos.x };

            const auto r1 { pos.x + size.x };
            const auto r2 { other.pos.x + other.size.x };

            const auto t1 { pos.y };
            const auto t2 { other.pos.y };

            const auto b1 { pos.y + size.y };
            const auto b2 { other.pos.y + other.size.y };

            return r1 >= l2 && l1 <= r2 && t1 <= b2 && b1 >= t2;
        }

        constexpr operator SDL_Rect() const noexcept
        {
            using type = decltype(SDL_Rect::x);

            return SDL_Rect {
                static_cast<type>(pos.x),
                static_cast<type>(pos.y),
                static_cast<type>(size.x),
                static_cast<type>(size.y),
            };
        }

        constexpr operator SDL_FRect() const noexcept
        {
            using type = decltype(SDL_FRect::x);

            return SDL_FRect {
                static_cast<type>(pos.x),
                static_cast<type>(pos.y),
                static_cast<type>(size.x),
                static_cast<type>(size.y),
            };
        }
    };
}