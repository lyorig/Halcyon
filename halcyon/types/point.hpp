#pragma once

#include <halcyon/internal/SDL_types.hpp>
#include <lyo/cast.hpp>

#include <halcyon/internal/tags.hpp>

namespace hal
{
    template <lyo::arithmetic T>
    struct rectangle;

    template <lyo::arithmetic T>
    struct point
    {
        T x {}, y {};

        constexpr point<T> operator+(const point& pt) const
        {
            return point<T> {
                lyo::cast<T>(x + pt.x),
                lyo::cast<T>(y + pt.y)
            };
        }

        constexpr point<T> operator-(const point& pt) const
        {
            return point<T> {
                lyo::cast<T>(x - pt.x),
                lyo::cast<T>(y - pt.y)
            };
        }

        constexpr point& operator+=(const point& pt)
        {
            x += pt.x;
            y += pt.y;

            return *this;
        }

        constexpr point& operator-=(const point& pt)
        {
            x -= pt.x;
            y -= pt.y;

            return *this;
        }

        constexpr point operator*(lyo::f64 mul) const
        {
            return point { lyo::cast<T>(x * mul), lyo::cast<T>(y * mul) };
        }

        constexpr point operator/(lyo::f64 div) const
        {
            return point { lyo::cast<T>(x / div), lyo::cast<T>(y / div) };
        }

        constexpr point& operator*=(lyo::f64 mul)
        {
            x = lyo::cast<T>(static_cast<lyo::f64>(x) * mul);
            y = lyo::cast<T>(static_cast<lyo::f64>(y) * mul);

            return *this;
        }

        constexpr point& operator/=(lyo::f64 div)
        {
            x = lyo::cast<T>(static_cast<lyo::f64>(x) / div);
            y = lyo::cast<T>(static_cast<lyo::f64>(y) / div);

            return *this;
        }

        constexpr point operator-() const
            requires std::is_signed_v<T>
        {
            return point {
                -x,
                -y
            };
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
        constexpr operator point<Convert>() const
        {
            return point<Convert> { lyo::cast<Convert>(x),
                lyo::cast<Convert>(y) };
        }

        constexpr operator SDL_Point() const
        {
            using t = decltype(SDL_Point::x);

            return SDL_Point {
                lyo::cast<t>(x),
                lyo::cast<t>(y)
            };
        }

        constexpr operator SDL_FPoint() const
        {
            using t = decltype(SDL_FPoint::x);

            return SDL_FPoint {
                lyo::cast<t>(x),
                lyo::cast<t>(y)
            };
        }

        constexpr SDL::point_t<T>* addr()
            requires(lyo::is_present_v<T, SDL::pixel_t, SDL::coord_t>)
        {
            return reinterpret_cast<SDL::point_t<T>*>(this);
        }

        constexpr const SDL::point_t<T>* addr() const
            requires(lyo::is_present_v<T, SDL::pixel_t, SDL::coord_t>)
        {
            return reinterpret_cast<const SDL::point_t<T>*>(this);
        }
    };

    template <lyo::arithmetic T>
    bool operator==(const point<T>& lhs, const point<T> rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    namespace SDL
    {
        using Point  = point<SDL::pixel_t>;
        using FPoint = point<SDL::coord_t>;
    }

} // namespace hal