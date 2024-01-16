#pragma once

#include <algorithm>
#include <halcyon/debug.hpp>
#include <lyo/cast.hpp>
#include <lyo/types.hpp>

// slider.hpp:
// A bounded "sliding value" with a modifier.

namespace hal
{
    // Slider Functor Placeholder Type.
    struct SFPT
    {
        lyo::f64 operator()(lyo::f64 val [[maybe_unused]]) const
        {
            HAL_PANIC("SFPT functor was called");
            return 69.420;
        }
    };

    // The functor receives a value 0.0 - 1.0 and returns a value in the same range.
    template <lyo::arithmetic T, auto Functor = SFPT {}>
        requires(requires(lyo::f64 val) { static_cast<void>(Functor(val)); })
    class slider
    {
    public:
        constexpr slider(T min, T max)
            : m_min { min }
            , m_max { max }
            , m_value { min }
        {
            HAL_ASSERT(min < max, "Slider min is >= max");
        }

        // Adds the modifier and returns the new value.
        constexpr T update(lyo::f64 delta)
        {
            this->operator+=(m_mod * delta);
            return this->value();
        }

        constexpr T value() const
        {
            if constexpr (!std::is_same_v<decltype(Functor), SFPT>)
                return Functor(this->progress()) * this->range() + this->min();

            else
                return m_value;
        }

        // Returns 0.0 - 1.0. Used for the functor.
        lyo::f64 progress() const
        {
            return (m_value - min()) / this->range();
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
            m_value = std::clamp(val, m_min, m_max);
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

    // A slider with static bounds. Min/Max values have to be "auto" due to floating
    // point template parameters causing a compilation error.
    // The functor receives a value 0.0 - 1.0 and returns a value in the same range.
    template <lyo::arithmetic T, auto Min, auto Max, auto Functor = SFPT {}>
        requires(std::convertible_to<decltype(Min), T> && std::convertible_to<decltype(Max), T> && Min < Max && requires(lyo::f64 val) { static_cast<void>(Functor(val)); })
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
        constexpr T update(lyo::f64 delta)
        {
            this->operator+=(m_mod * delta);
            return this->value();
        }

        constexpr T value() const
        {
            if constexpr (!std::is_same_v<decltype(Functor), SFPT>)
                return Functor(this->progress()) * range() + min();

            else
                return m_value;
        }

        // Returns 0.0 - 1.0. Used for the functor.
        lyo::f64 progress() const
        {
            return (m_value - min()) / range();
        }

        consteval static T min()
        {
            return Min;
        }

        consteval static T max()
        {
            return Max;
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
            m_value = std::clamp(val, min(), max());
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