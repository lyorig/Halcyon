#pragma once

#include <lyo/timer.hpp>
#include <quest/states/base.hpp>

// intro.hpp:
// The first thing that happens when the game loads.
// This class is a one-off, so it doesn't store its state in the game object.

namespace quest
{
    namespace state
    {
        class intro final : public base
        {
        public:
            type update(const input_type& input, lyo::f64 elapsed);

        private:
            lyo::steady_timer m_tmr;

            // This class does something highly immoral - see intro.cpp.
            ~intro() = default;
        };
    }
}