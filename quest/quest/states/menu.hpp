#pragma once

#include <lyo/timer.hpp>
#include <quest/states/base.hpp>

namespace quest
{
    namespace state
    {
        class menu final : public base
        {
        public:
            type update(const input_type& input, lyo::f64 elapsed);

        private:
            lyo::steady_timer m_tmr;
        };
    }
}