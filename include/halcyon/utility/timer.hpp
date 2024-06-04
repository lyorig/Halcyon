#pragma once

#include <chrono>

#include <halcyon/types/numeric.hpp>

// timer.hpp:
// Timing facilities.

namespace hal
{
    // A simple timer for all your timing needs. It'll keep running no matter what.
    class timer
    {
    public:
        using clock = std::chrono::steady_clock;

        // Create a timer that begins at the time of construction.
        timer();

        void reset();

        f64 operator()() const;

        template <typename Rep, typename Period>
        timer& operator=(std::chrono::duration<Rep, Period> dur)
        {
            reset();
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator+=(std::chrono::duration<Rep, Period> dur)
        {
            m_epoch -= dur;
            return *this;
        }

        template <typename Rep, typename Period>
        timer& operator-=(std::chrono::duration<Rep, Period> dur)
        {
            m_epoch += dur;
            return *this;
        }

        clock::time_point time_point() const
        {
            return m_epoch;
        }

    private:
        clock::time_point m_epoch;
    };
}