#pragma once

#include <algorithm>
#include <halcyon/debug.hpp>
#include <quest/types.hpp>

namespace quest
{
    namespace easing
    {
        constexpr delta_t linear(delta_t t) { return t; }

        namespace in // TODO
        {

        }

        namespace out // TODO
        {

        }

        namespace in_out
        {
            constexpr delta_t bezier(delta_t t) { return t * t * (3.0 - 2.0 * t); }

            constexpr delta_t parametric(delta_t t)
            {
                const delta_t sqr { t * t };
                return sqr / (2.0 * (sqr - t) + 1.0);
            }
        }
    }

    template <typename T, auto AnimFunc>
        requires std::is_invocable_r_v<delta_t, decltype(AnimFunc), delta_t>
    class animatable
    {
    public:
        animatable() = default;

        animatable(const T& initial)
            : m_start { initial }
        {
        }

        void update(delta_t elapsed)
        {
            if (animating())
            {
                m_elapsed = std::min(m_elapsed + elapsed, m_time);
            }
        }

        void start(const T& val, delta_t time)
        {
            HAL_ASSERT(time >= 0.0, "Negative animation duration");

            m_start = value();
            target(val);

            m_time    = time;
            m_elapsed = 0.0;
        }

        // "Teleport" instantly to a value.
        void jump(const T& val)
        {
            start(val, 0.0);
        }

        // Change the target.
        // This is only really useful when retargeting mid-animation.
        void target(const T& val)
        {
            m_dist = val - m_start;
        }

        T value() const
        {
            return m_start + m_dist * AnimFunc(m_time == 0.0 ? 1.0 : m_elapsed / m_time);
        }

        bool animating() const
        {
            return m_elapsed != m_time;
        }

    private:
        T m_start {},
            m_dist {};

        delta_t m_time { 0.0 },
            m_elapsed { m_time };
    };
}