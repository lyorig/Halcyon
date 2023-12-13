#pragma once

#include <algorithm>
#include <cassert>

#include "concepts.hpp"

/* slider.hpp:
   An arithmetic value with a minimum and maximum value. */

namespace lyo
{
    template <lyo::arithmetic T>
    class slider
    {
    public:
        constexpr slider(T min, T max)
            : m_value { static_cast<T>((min + max) / 2.0) }
            , m_min { min }
            , m_max { max }
        {
            assert(min < max);
        }

        constexpr slider(T min, T max, T value) noexcept
            : m_value { std::clamp(value, min, max) }
            , m_min { min }
            , m_max { max }
        {
            assert(min < max);
        }

        constexpr explicit operator T() const noexcept
        {
            return m_value;
        }

        constexpr T min() const noexcept
        {
            return m_min;
        }

        constexpr T max() const noexcept
        {
            return m_max;
        }

        constexpr bool on_border() const noexcept
        {
            return m_value == m_min || m_value == m_max;
        }

        constexpr slider& set_min(T value) noexcept
        {
            m_value = std::max(m_value, m_min = value);

            return *this;
        }

        constexpr slider& set_max(T value) noexcept
        {
            m_value = std::min(m_value, m_max = value);

            return *this;
        }

        constexpr slider& operator=(T value) noexcept
        {
            m_value = std::clamp(value, m_min, m_max);

            return *this;
        }

        constexpr slider& operator+=(T add) noexcept
        {
            m_value = std::clamp(m_value + add, m_min, m_max);

            return *this;
        }

        constexpr slider& operator-=(T subtract) noexcept
        {
            m_value = std::clamp(m_value - subtract, m_min, m_max);

            return *this;
        }

    private:
        T m_value, m_min, m_max;
    };

    template <lyo::arithmetic T, T Min, T Max>
        requires(Min < Max)
    class static_slider
    {
    public:
        constexpr static_slider() noexcept
            : m_value { static_cast<T>((Min + Max) / 2.0) }
        {
        }

        constexpr static_slider(T value) noexcept
            : m_value { std::clamp(value, Min, Max) }
        {
        }

        constexpr operator T() const noexcept
        {
            return m_value;
        }

        constexpr T min() const noexcept
        {
            return Min;
        }

        constexpr T max() const noexcept
        {
            return Max;
        }

        constexpr bool on_border() const noexcept
        {
            return m_value == Min || m_value == Max;
        }

        constexpr static_slider& operator=(T value) noexcept
        {
            m_value = std::clamp(value, Min, Max);

            return *this;
        }

        constexpr static_slider& operator+=(T add) noexcept
        {
            m_value = std::clamp(m_value + add, Min, Max);

            return *this;
        }

        constexpr static_slider& operator-=(T subtract) noexcept
        {
            m_value = std::clamp(m_value - subtract, Min, Max);

            return *this;
        }

    private:
        T m_value;
    };
} // namespace lyo