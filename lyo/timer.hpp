#pragma once

#include <chrono>
#include <lyo/types.hpp>

/* timer.hpp:
   A lightweight and simple timing system. */

namespace lyo
{
    template <typename T>
    concept clock = std::chrono::is_clock_v<T>;

    template <lyo::clock Clock>
    class timer
    {
        using second_type = std::chrono::duration<lyo::f64>;

      public:

        timer() noexcept :
            m_epoch { Clock::now() }
        {
        }

        timer& reset() noexcept
        {
            m_epoch = Clock::now();

            return *this;
        }

        f64 operator()() const noexcept
        {
            return std::chrono::duration<f64> { Clock::now() - m_epoch }.count();
        }

        timer& operator=(f64 time) noexcept
        {
            m_epoch = Clock::now() - second_type { time };

            return *this;
        }

        timer& operator+=(f64 time) noexcept
        {
            m_epoch -= second_type { time };

            return *this;
        }

        timer& operator-=(f64 time) noexcept
        {
            m_epoch += second_type { time };

            return *this;
        }

      private:

        std::chrono::time_point<Clock, std::chrono::duration<f64, typename Clock::period>> m_epoch;
    };

    using steady_timer  = timer<std::chrono::steady_clock>;
    using system_timer  = timer<std::chrono::system_clock>;
    using precise_timer = timer<std::chrono::high_resolution_clock>;
}  // namespace lyo