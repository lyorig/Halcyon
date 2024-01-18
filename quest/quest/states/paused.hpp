#pragma once

#include <quest/states/base.hpp>

namespace quest
{
    namespace state
    {
        class paused final : public base
        {
        public:
            type update(const input_type& input, lyo::f64 delta);
        };
    }
}