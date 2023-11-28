#pragma once

#include <lyo/cast.hpp>
#include <lyo/concepts.hpp>

#include <SDL2/SDL_rect.h>

#include "tags.hpp"

namespace hal
{
    template <lyo::arithmetic T>
    struct rectangle;

    template <lyo::arithmetic T>
    struct point
    {
        T x {}, y {};

        constexpr point<T> operator+(const point& pt) const noexcept
        {
            return point<T> { x + pt.x, y + pt.y };
        }

        constexpr point operator*(lyo::f64 mul) const noexcept
        {
            return point { lyo::round_cast<T>(x * mul), lyo::round_cast<T>(y * mul) };
        }

        constexpr point operator/(lyo::f64 div) const noexcept
        {
            return point { lyo::round_cast<T>(x / div), lyo::round_cast<T>(y / div) };
        }

        constexpr point& operator*=(lyo::f64 mul) const noexcept
        {
            x *= mul;
            y *= mul;

            return *this;
        }

        constexpr point& operator/=(lyo::f64 div) const noexcept
        {
            x /= div;
            y /= div;

            return *this;
        }

        // Create a new rectangle with this point acting as the size.
        constexpr rectangle<T> rect() const noexcept
        {
            return rectangle<T> { as_size, *this };
        }

        // Join two points into a rectangle.
        constexpr rectangle<T> rect(const point<T>& pt) const noexcept
        {
            return rectangle<T> { *this, pt };
        }

        template <lyo::arithmetic Convert>
        constexpr explicit operator point<Convert>() const noexcept
        {
            return point<Convert> { lyo::round_cast<Convert>(x),
                lyo::round_cast<Convert>(y) };
        }

        constexpr operator SDL_Point() const noexcept
        {
            using t = decltype(SDL_Point::x);

            return SDL_Point {
                lyo::round_cast<t>(x),
                lyo::round_cast<t>(y)
            };
        }

        constexpr operator SDL_FPoint() const noexcept
        {
            using t = decltype(SDL_FPoint::x);

            return SDL_FPoint {
                lyo::round_cast<t>(x),
                lyo::round_cast<t>(y)
            };
        }
    };
}  // namespace hal