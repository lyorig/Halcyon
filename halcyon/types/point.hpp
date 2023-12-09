#pragma once

#include <halcyon/internal/SDL_types.hpp>
#include <lyo/cast.hpp>

#include "tags.hpp"

namespace hal {
template <lyo::arithmetic T>
struct rectangle;

template <lyo::arithmetic T>
struct point {
    T x {}, y {};

    constexpr point<T> operator+(const point& pt) const
    {
        return point<T> {
            lyo::round_cast<T>(x + pt.x),
            lyo::round_cast<T>(y + pt.y)
        };
    }

    constexpr point<T> operator-(const point& pt) const
    {
        return point<T> {
            lyo::round_cast<T>(x - pt.x),
            lyo::round_cast<T>(y - pt.y)
        };
    }

    constexpr point operator*(lyo::f64 mul) const
    {
        return point { lyo::round_cast<T>(x * mul), lyo::round_cast<T>(y * mul) };
    }

    constexpr point operator/(lyo::f64 div) const
    {
        return point { lyo::round_cast<T>(x / div), lyo::round_cast<T>(y / div) };
    }

    constexpr point& operator*=(lyo::f64 mul)
    {
        x *= mul;
        y *= mul;

        return *this;
    }

    constexpr point& operator/=(lyo::f64 div)
    {
        x /= div;
        y /= div;

        return *this;
    }

    // Create a new rectangle with this point acting as the size.
    constexpr rectangle<T> rect() const
    {
        return rectangle<T> { as_size, *this };
    }

    // Join two points into a rectangle.
    constexpr rectangle<T> rect(const point<T>& pt) const
    {
        return rectangle<T> { *this, pt };
    }

    template <lyo::arithmetic Convert>
    constexpr explicit operator point<Convert>() const
    {
        return point<Convert> { lyo::round_cast<Convert>(x),
            lyo::round_cast<Convert>(y) };
    }

    constexpr operator SDL_Point() const
    {
        using t = decltype(SDL_Point::x);

        return SDL_Point {
            lyo::round_cast<t>(x),
            lyo::round_cast<t>(y)
        };
    }

    constexpr operator SDL_FPoint() const
    {
        using t = decltype(SDL_FPoint::x);

        return SDL_FPoint {
            lyo::round_cast<t>(x),
            lyo::round_cast<t>(y)
        };
    }

    constexpr SDL::point_type<T>* addr()
        requires(lyo::is_any_of<T, SDL::pixel_type, SDL::position_type>())
    {
        return reinterpret_cast<SDL::point_type<T>*>(this);
    }

    constexpr const SDL::point_type<T>* addr() const
        requires(lyo::is_any_of<T, SDL::pixel_type, SDL::position_type>())
    {
        return reinterpret_cast<const SDL::point_type<T>*>(this);
    }
};

using point_wrap = point<SDL::pixel_type>;
using fpoint_wrap = point<SDL::position_type>;
} // namespace hal