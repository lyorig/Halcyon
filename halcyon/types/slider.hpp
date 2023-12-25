#pragma once

#include <algorithm>
#include <lyo/cast.hpp>

// slider.hpp:
// A bounded "sliding value" with a modifier.

namespace hal
{
    template <lyo::arithmetic T>
    class slider
    {
    public:
        constexpr slider(T min, T max)
            : m_min { min }
            , m_max { max }
            , m_value { min }
        {
            assert(min <= max);
        }

        // Adds the modifier and returns the new value.
        constexpr T update()
        {
            this->operator+=(m_mod);
            return this->value();
        }

        constexpr T value() const
        {
            return m_value;
        }

        constexpr T min() const
        {
            return m_min;
        }

        constexpr T max() const
        {
            return m_max;
        }

        constexpr T range() const
        {
            return this->max() - this->min();
        }

        constexpr void set_min(T val)
        {
            m_min = val;
        }

        constexpr void set_max(T val)
        {
            m_max = val;
        }

        constexpr void set_mod(T val)
        {
            m_mod = val;
        }

        constexpr slider& operator=(T val)
        {
            m_value = std::clamp(m_value, m_min, m_max);
            return *this;
        }

        constexpr slider& operator+=(T add)
        {
            m_value = std::clamp(m_value + add, m_min, m_max);
            return *this;
        }

        constexpr slider& operator-=(T sub)
        {
            m_value = std::clamp(m_value - sub, m_min, m_max);
            return *this;
        }

        constexpr slider& operator*=(T mul)
        {
            m_value = std::clamp(m_value * mul, m_min, m_max);
            return *this;
        }

        constexpr slider& operator/=(T div)
        {
            m_value = std::clamp(m_value / div, m_min, m_max);
            return *this;
        }

    private:
        T m_min {};
        T m_max {};
        T m_value {};
        T m_mod {};
    };

    template <lyo::arithmetic T, auto Min, auto Max>
        requires(lyo::arithmetic<decltype(Min)> && lyo::arithmetic<decltype(Max)> && Min <= Max)
    class static_slider
    {
    public:
        constexpr static_slider()
            : m_value { Min }
        {
        }

        constexpr static_slider(T value)
            : m_value { value }
        {
        }

        // Adds the modifier and returns the new value.
        constexpr T update()
        {
            this->operator+=(m_mod);
            return this->value();
        }

        constexpr T value() const
        {
            return m_value;
        }

        consteval static T min()
        {
            return T(Min);
        }

        consteval static T max()
        {
            return T(Max);
        }

        consteval static T range()
        {
            return max() - min();
        }

        constexpr void set_mod(T val)
        {
            m_mod = val;
        }

        constexpr static_slider& operator=(T val)
        {
            m_value = std::clamp(m_value, min(), max());
            return *this;
        }

        constexpr static_slider& operator+=(T add)
        {
            m_value = std::clamp(m_value + add, min(), max());
            return *this;
        }

        constexpr static_slider& operator-=(T sub)
        {
            m_value = std::clamp(m_value - sub, min(), max());
            return *this;
        }

        constexpr static_slider& operator*=(T mul)
        {
            m_value = std::clamp(m_value * mul, min(), max());
            return *this;
        }

        constexpr static_slider& operator/=(T div)
        {
            m_value = std::clamp(m_value / div, min(), max());
            return *this;
        }

    private:
        T m_value {};
        T m_mod {};
    };
}