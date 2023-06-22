#pragma once

#include <chrono>
#include <lyo/types.hpp>
#include <variant>

/* timer.hpp:
   A lightweight and simple timing system. */

namespace lyo
{
    template <typename T>
    concept clock = std::chrono::is_clock_v<T>;

    template <lyo::clock Clock>
    class timer
    {
      public:

        timer() noexcept :
            m_data { Clock::now() }
        {
        }

        timer& reset() noexcept
        {
            m_data = Clock::now();

            return *this;
        }

        void pause() noexcept
        {
            if (std::holds_alternative<tp>(m_data))
                m_data = std::chrono::duration<f64> { Clock::now() - std::get<tp>(m_data) }.count();
        }

        void resume() noexcept
        {
            if (std::holds_alternative<double>(m_data))
                m_data = Clock::now() - second_type { std::get<double>(m_data) };
        }

        f64 operator()() const noexcept
        {
            if (std::holds_alternative<tp>(m_data))  // Not paused (time-point).
                return std::chrono::duration<f64> { Clock::now() - std::get<tp>(m_data) }.count();

            else  // Paused (double).
                return std::get<double>(m_data);
        }

        timer& operator=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data))  // Not paused (time-point).
                m_data = Clock::now() - second_type { time };

            else  // Paused (double).
                std::get<double>(m_data) = time;

            return *this;
        }

        timer& operator+=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data))  // Not paused (time-point).
                m_data -= second_type { time };

            else  // Paused (double).
                std::get<double>(m_data) += time;

            return *this;
        }

        timer& operator-=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data))  // Not paused (time-point).
                m_data += second_type { time };

            else  // Paused (double).
                std::get<double>(m_data) -= time;

            return *this;
        }

      private:

        using second_type = std::chrono::duration<lyo::f64>;
        using tp          = std::chrono::time_point<Clock, std::chrono::duration<f64, typename Clock::period>>;

        std::variant<tp, double> m_data;
    };

    using steady_timer  = timer<std::chrono::steady_clock>;
    using system_timer  = timer<std::chrono::system_clock>;
    using precise_timer = timer<std::chrono::high_resolution_clock>;
}  // namespace lyo