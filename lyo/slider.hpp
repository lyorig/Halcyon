#pragma once

#include <algorithm>

#include "concepts.hpp"

/* slider.hpp:
   A variable with a minimum and maximum value. */

namespace lyo
{
    template <lyo::arithmetic T>
    class slider
    {
      public:

        constexpr slider(T lower_bound, T upper_bound) :
            m_value { (lower_bound + upper_bound) / 2.0 },
            m_min { lower_bound },
            m_max { upper_bound }
        {
        }

        constexpr slider(T lower_bound, T upper_bound, T value) noexcept :
            m_value { std::clamp(value, lower_bound, upper_bound) },
            m_min { lower_bound },
            m_max { upper_bound }
        {
        }

        operator T() const noexcept
        {
            return m_value;
        }

        T min() const noexcept
        {
            return m_min;
        }

        T max() const noexcept
        {
            return m_max;
        }

        bool on_border() const noexcept
        {
            return m_value == m_min || m_value == m_max;
        }

        slider& set_min(T value) noexcept
        {
            m_value = std::max(m_value, m_min = value);

            return *this;
        }

        slider& set_max(T value) noexcept
        {
            m_value = std::min(m_value, m_max = value);

            return *this;
        }

        slider& operator=(T value) noexcept
        {
            m_value = std::clamp(value, m_min, m_max);

            return *this;
        }

        slider& operator+=(T add) noexcept
        {
            m_value = std::clamp(m_value + add, m_min, m_max);

            return *this;
        }

        slider& operator-=(T subtract) noexcept
        {
            m_value = std::clamp(m_value - subtract, m_min, m_max);

            return *this;
        }

      private:

        T m_value, m_min, m_max;  // sizeof(T) * 3b
    };

    /* Todo: Fix this nonstandard floating-point template parameter bullshit */
    template <lyo::arithmetic T, T Min, T Max>
    class static_slider
    {
      public:

        constexpr static_slider(T value = static_cast<T>((Min + Max) / 2.0)) noexcept :
            m_value { std::clamp(value, Min, Max) }
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
}  // namespace lyo