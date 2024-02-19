#pragma once

#include <SDL.h>
#include <string_view>

// engine.hpp:
// The core of Halcyon, without which nothing can be initialized.

namespace hal
{
    class engine
    {
    public:
        engine();
        ~engine();

        void exit() &;

        static std::string_view driver_name();

    private:
        void deinitialize() const;
    };
} // namespace halcyon