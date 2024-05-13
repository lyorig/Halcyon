#pragma once

#include <compare>

#include <halcyon/internal/sdl_types.hpp>
#include <halcyon/internal/tags.hpp>

// types/point.hpp:
// A 2D point class, there's really not much more to say.

namespace hal
{
    enum class anchor : u8
    {
        center,
        top_left,
        top_right,
        bottom_left,
        bottom_right
    };

    template <meta::arithmetic T>
    struct rectangle;

    template <meta::arithmetic T>
    struct point
    {
        T x {}, y {};

        // In-place arithmetic operations.
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

        constexpr point& operator*=(const point& mul)
        {
            x *= mul.x;
            y *= mul.y;

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
            if constexpr (std::is_floating_point_v<T>)
            {
                x = std::fmod(x, mod.x);
                y = std::fmod(y, mod.y);
            }

            else
            {
                x %= mod.x;
                y %= mod.y;
            }

            return *this;
        }

        // Arithmetic operations.
        template <meta::arithmetic Add>
        constexpr auto operator+(const point<Add>& pt) const
        {
            point<decltype(x + pt.x)> ret = *this;
            ret += pt;
            return ret;
        }

        template <meta::arithmetic Mns>
        constexpr auto operator-(const point<Mns>& pt) const
        {
            point<decltype(x - pt.x)> ret = *this;
            ret -= pt;
            return ret;
        }

        template <meta::arithmetic Mul>
        constexpr auto operator*(const point<Mul>& pt) const
        {
            point<decltype(x * pt.x)> ret { *this };
            ret *= pt;
            return ret;
        }

        template <meta::arithmetic Div>
        constexpr auto operator/(const point<Div>& pt) const
        {
            point<decltype(x / pt.x)> ret { *this };
            ret /= pt;
            return ret;
        }

        template <meta::arithmetic Mod>
        constexpr auto operator%(const point<Mod>& pt) const
        {
            point<decltype(x % pt.x)> ret { *this };
            ret %= pt;
            return ret;
        }

        constexpr point operator-() const
            requires std::is_signed_v<T>
        {
            return point {
                static_cast<T>(-x),
                static_cast<T>(-y)
            };
        }

        // Additional arithmetic operations.
        template <meta::arithmetic Mul>
        constexpr auto operator*(Mul mul) const
        {
            return point<decltype(x * mul)> {
                x * mul,
                y * mul
            };
        }

        template <meta::arithmetic Div>
        constexpr auto operator/(Div div) const
        {
            return point<decltype(x / div)> {
                x / div,
                y / div
            };
        }

        constexpr point abs() const
            requires std::is_signed_v<T>
        {
            return {
                static_cast<T>(std::abs(x)),
                static_cast<T>(std::abs(y))
            };
        }

        // Get the origin of a rectangle anchored by a certain corner in this point.
        constexpr point anchor(anchor a, const point& sz) const
        {
            point ret { *this };

            using enum hal::anchor;

            switch (a)
            {
            case center:
                ret -= sz / 2;
                break;

            case top_left:
                // Nothing to do.
                break;

            case top_right:
                ret.x += sz.x;
                break;

            case bottom_left:
                ret.y += sz.y;
                break;

            case bottom_right:
                ret += sz;
                break;
            }

            return ret;
        }

        // Comparisons.
        constexpr auto operator<=>(const point& cmp) const = default;

        // Conversions.
        template <meta::arithmetic Convert>
        constexpr operator point<Convert>() const
        {
            return point<Convert> { static_cast<Convert>(x),
                static_cast<Convert>(y) };
        }

        constexpr sdl::point_t<T>* addr()
            requires(meta::is_present<T, sdl::pixel_t, sdl::coord_t>)
        {
            return reinterpret_cast<sdl::point_t<T>*>(this);
        }

        constexpr const sdl::point_t<T>* addr() const
            requires(meta::is_present<T, sdl::pixel_t, sdl::coord_t>)
        {
            return reinterpret_cast<const sdl::point_t<T>*>(this);
        }
    };

    extern template struct point<std::int8_t>;
    extern template struct point<std::int16_t>;
    extern template struct point<std::int32_t>;
    extern template struct point<std::int64_t>;

    extern template struct point<std::uint8_t>;
    extern template struct point<std::uint16_t>;
    extern template struct point<std::uint32_t>;
    extern template struct point<std::uint64_t>;

    extern template struct point<float>;
    extern template struct point<double>;

    template <meta::arithmetic T>
    constexpr f64 distance(const point<T>& lhs, const point<T>& rhs)
    {
        const point<T> dist { lhs - rhs };
        return static_cast<f64>(std::sqrt(dist.x * dist.x + dist.y * dist.y));
    }

    // Wrappers for native SDL types.
    namespace sdl
    {
        using pixel_point = point<sdl::pixel_t>;
        using coord_point = point<sdl::coord_t>;
    }
}