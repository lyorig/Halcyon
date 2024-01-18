#pragma once

#include <halcyon/input_handler.hpp>

namespace quest
{
    namespace state
    {
        // This enum only encompasses the states that can be
        // jumped into, so some (for example intro) are left out.
        enum class type
        {
            none, // Used for no transition.
            menu,
            playing,
            paused
        };

        class base
        {
        protected:
            using input_type = hal::queued_input_handler;

        public:
            virtual ~base();

            virtual type update(const input_type& input, lyo::f64 elapsed) = 0;
        };
    }
}