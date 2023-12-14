#pragma once

#include <chrono>
#include <lyo/types.hpp>
#include <variant>

/* timer.hpp:
   Timing facilities. */

namespace lyo
{
    TAG_TYPE(no_init);

    // A simple timer for all your timing needs. It'll keep
    // running no matter what, so if you require pause/resume
    // functionality, use the stopwatch instead.
    template <typename Clock>
    class timer
    {
    public:
        // Create a timer that begins at the time of construction.
        timer() noexcept
            : m_epoch { Clock::now() }
        {
        }

        // Create an uninitialized timer.
        timer(no_init_tag) noexcept
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
        using second_type = std::chrono::duration<f64>;
        using tp = std::chrono::time_point<Clock, second_type>;

        tp m_epoch;
    };

    using steady_timer = timer<std::chrono::steady_clock>;
    using system_timer = timer<std::chrono::system_clock>;
    using precise_timer = timer<std::chrono::high_resolution_clock>;

    // A pausable timer, essentially. This incurs a bit of both
    // space and runtime overhead, so if you intend to keep it
    // running all the time, use the regular timer instead.
    template <typename Clock>
    class stopwatch
    {
    public:
        // HALFIX: A raw union and a bool could be better.

        stopwatch() noexcept
            : m_data { Clock::now() }
        {
        }

        stopwatch& reset() noexcept
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
            if (std::holds_alternative<f64>(m_data))
                m_data = Clock::now() - second_type { std::get<f64>(m_data) };
        }

        f64 operator()() const noexcept
        {
            if (std::holds_alternative<tp>(m_data)) // Not paused (time-point).
                return std::chrono::duration<f64> { Clock::now() - std::get<tp>(m_data) }.count();

            else // Paused (double).
                return std::get<f64>(m_data);
        }

        stopwatch& operator=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data)) // Not paused (time-point).
                m_data = Clock::now() - second_type { time };

            else // Paused (double).
                std::get<f64>(m_data) = time;

            return *this;
        }

        stopwatch& operator+=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data)) // Not paused (time-point).
                m_data -= second_type { time };

            else // Paused (double).
                std::get<lyo::f64>(m_data) += time;

            return *this;
        }

        stopwatch& operator-=(f64 time) noexcept
        {
            if (std::holds_alternative<tp>(m_data)) // Not paused (time-point).
                m_data += second_type { time };

            else // Paused (double).
                std::get<f64>(m_data) -= time;

            return *this;
        }

    private:
        using second_type = std::chrono::duration<f64>;
        using tp = std::chrono::time_point<Clock, second_type>;

        std::variant<tp, f64> m_data;
    };

    using steady_stopwatch = stopwatch<std::chrono::steady_clock>;
    using system_stopwatch = stopwatch<std::chrono::system_clock>;
    using precise_stopwatch = stopwatch<std::chrono::high_resolution_clock>;
} // namespace lyo