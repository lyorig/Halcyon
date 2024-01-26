#pragma once

#include <climits>
#include <compare>
#include <halcyon/internal/sdl_types.hpp>
#include <halcyon/internal/tags.hpp>
#include <lyo/cast.hpp>

namespace hal
{
    template <lyo::arithmetic T>
    struct rectangle;

    template <lyo::arithmetic T>
    struct point
    {
        T x {}, y {};

        constexpr point operator+(const point& pt) const
        {
            point ret { *this };
            ret += pt;
            return ret;
        }

        constexpr point<T> operator-(const point& pt) const
        {
            point ret { *this };
            ret -= pt;
            return ret;
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

        template <lyo::arithmetic M>
        constexpr auto operator*(M mul) const -> point<decltype(x * mul)>
        {
            point<decltype(x * mul)> ret = *this;
            ret *= mul;
            return ret;
        }

        constexpr point operator*(const point& mul) const
        {
            point ret { *this };
            ret *= mul;
            return ret;
        }

        template <lyo::arithmetic D>
        constexpr auto operator/(D div) const -> point<decltype(x / div)>
        {
            point<decltype(x / div)> ret = *this;
            ret /= div;
            return ret;
        }

        constexpr point operator/(const point& div) const
        {
            point ret { *this };
            ret /= div;
            return ret;
        }

        constexpr point operator%(const point& mod) const
        {
            point ret { *this };
            ret %= mod;
            return ret;
        }

        constexpr point& operator*=(lyo::f64 mul)
        {
            // Bit verbose, but MSVC won't shut up otherwise.
            x = lyo::cast<T>(static_cast<lyo::f64>(x) * mul);
            y = lyo::cast<T>(static_cast<lyo::f64>(y) * mul);

            return *this;
        }

        constexpr point& operator*=(const point& mul)
        {
            x *= mul.x;
            y *= mul.y;

            return *this;
        }

        constexpr point& operator/=(lyo::f64 div)
        {
            // Same problem as in operator*=.
            x = lyo::cast<T>(static_cast<lyo::f64>(x) / div);
            y = lyo::cast<T>(static_cast<lyo::f64>(y) / div);

            return *this;
        }

        constexpr point& operator/=(const point& div)
        {
            x /= div.x;
            y /= div.y;

            return *this;
        }

        constexpr point& operator%=(const point& mod)
        {
            x %= mod.x;
            y %= mod.y;

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

        constexpr auto operator<=>(const point& cmp) const = default;

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

        constexpr sdl::point_t<T>* addr()
            requires(lyo::is_present_v<T, sdl::pixel_t, sdl::coord_t>)
        {
            return reinterpret_cast<sdl::point_t<T>*>(this);
        }

        constexpr const sdl::point_t<T>* addr() const
            requires(lyo::is_present_v<T, sdl::pixel_t, sdl::coord_t>)
        {
            return reinterpret_cast<const sdl::point_t<T>*>(this);
        }
    };

    template <lyo::arithmetic T>
    bool operator==(const point<T>& lhs, const point<T> rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    // Wrappers for native SDL types.
    namespace sdl
    {
        using pixel_point = point<sdl::pixel_t>;
        using coord_point = point<sdl::coord_t>;
    }

} // namespace hal