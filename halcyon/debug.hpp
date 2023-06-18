#pragma once

/* debug.hpp:
   Various debugging functions. Also hopefully the only
   part of Halcyon that uses preprocessor #defines. */

#include "console.hpp"

#ifndef NDEBUG
    #include <fstream>
    #include <iomanip>
    #include <iostream>
    #include <lyo/timer.hpp>
    #include <sstream>
#endif

namespace hal
{
    class debug
    {
      public:

#ifndef NDEBUG
        // Output any amount of arguments to stdout/stderr, the console and an output file.
        template <typename... Args>
        static void print(severity type, Args&&... args) noexcept
        {
            std::stringstream fwd_info, message;

            fwd_info << std::fixed << std::setprecision(3) << '[' << m_timer() << "s]\t";

            switch (type)
            {
                case info:
                    fwd_info << "[info]\t";
                    break;

                case success:
                    fwd_info << "[success]\t";
                    break;

                case warning:
                    fwd_info << "[warning]\t";
                    break;

                case error:
                    fwd_info << "[error]\t";
                    break;
            }

            // Fold expression to properly output every argument.
            (message << ... << args);

            const std::string msg { message.str() };

            console::log(static_cast<severity>(type), msg);

            const std::string with_info { fwd_info.str() + msg };

            m_output << with_info << std::endl;
            (type == error ? std::cerr : std::cout) << with_info << std::endl;
        }

        // Show a message box with an error message.
        static void panic(const char* title, const char* message) noexcept;

        // Check a condition, and panic if it's false.
        static void verify(bool condition, const char* func, const char* info) noexcept;

      private:

        static std::ofstream            m_output;
        static const lyo::precise_timer m_timer;
#endif
    };
}  // namespace hal

#ifndef NDEBUG

    #define HAL_DEBUG_PRINT                  hal::debug::print
    #define HAL_DEBUG_PANIC                  hal::debug::panic
    #define HAL_DEBUG_VERIFY(cond, if_false) hal::debug::verify(cond, #cond " eval'd to false", if_false)

#else

    #define HAL_DEBUG_PRINT(...)             (static_cast<void>(0))
    #define HAL_DEBUG_PANIC(...)             (static_cast<void>(0))
    #define HAL_DEBUG_VERIFY(condition, ...) (static_cast<void>(condition))  // Make sure functions with side effects get called.

#endif