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
        using second_type = std::chrono::duration<double>;

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

        double operator()() const noexcept
        {
            return std::chrono::duration<double> { Clock::now() - m_epoch }.count();
        }

        timer& operator+=(double time) noexcept
        {
            m_epoch -= second_type { time };

            return *this;
        }

        timer& operator-=(double time) noexcept
        {
            m_epoch += second_type { time };

            return *this;
        }

      private:

        std::chrono::time_point<Clock, std::chrono::duration<double, typename Clock::period>> m_epoch;
    };

    using steady_timer  = timer<std::chrono::steady_clock>;
    using system_timer  = timer<std::chrono::system_clock>;
    using precise_timer = timer<std::chrono::high_resolution_clock>;
}  // namespace lyo