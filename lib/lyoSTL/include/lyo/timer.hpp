#pragma once

#include <chrono>
#include <variant>

#include <lyo/tags.hpp>

// timer.hpp:
// Timing facilities.

namespace lyo
{
    // A simple timer for all your timing needs. It'll keep running no matter what.
    class timer
    {
    public:
        using clock = std::chrono::steady_clock;

        // Create a timer that begins at the time of construction.
        timer() noexcept
            : m_epoch { clock::now() }
        {
        }

        // Create an uninitialized timer.
        timer(tag::no_init_t) noexcept
        {
        }

        timer& reset() noexcept
        {
            m_epoch = clock::now();
            return *this;
        }

        f64 operator()() const noexcept
        {
            return std::chrono::duration<f64> { clock::now() - m_epoch }.count();
        }

        template <typename Rep, typename Period>
        timer& operator=(std::chrono::duration<Rep, Period> dur) noexcept
        {
            reset();
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator+=(std::chrono::duration<Rep, Period> dur) noexcept
        {
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator-=(std::chrono::duration<Rep, Period> dur) noexcept
        {
            m_epoch += dur;
            return *this;
        }

    private:
        using second_type = std::chrono::duration<f64>;
        using tp          = std::chrono::time_point<clock, second_type>;

        clock::time_point m_epoch;
    };
}