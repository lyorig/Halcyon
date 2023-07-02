#pragma once

#include <lyo/concepts.hpp>

namespace hal
{
    template <lyo::arithmetic T>
    struct rectangle;

    template <lyo::arithmetic T>
    struct point
    {
        T x {}, y {};

        constexpr rectangle<T> operator+(const point& pt) const noexcept
        {
            return rectangle<T> {
                *this,
                pt
            };
        }

        constexpr point operator*(double mul) const noexcept
        {
            return point {
                static_cast<T>(x * mul),
                static_cast<T>(y * mul)
            };
        }

        constexpr point operator/(double div) const noexcept
        {
            return point {
                x / div,
                y / div
            };
        }

        constexpr point& operator*=(double mul) const noexcept
        {
            x *= mul;
            y *= mul;

            return *this;
        }

        constexpr point& operator/=(double div) const noexcept
        {
            x /= div;
            y /= div;

            return *this;
        }

        constexpr rectangle<T> rect() const noexcept
        {
            return rectangle<T> {
                *this
            };
        }

        template <lyo::arithmetic Convert>
        constexpr explicit operator point<Convert>() const noexcept
        {
            return point<Convert> {
                static_cast<Convert>(x),
                static_cast<Convert>(y)
            };
        }
    };
}  // namespace hal