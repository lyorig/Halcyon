#pragma once

/* debug.hpp:
   Various debugging functions. Also hopefully the only
   part of Halcyon that uses preprocessor #defines. */

#ifndef NDEBUG
    #include <fstream>
    #include <iomanip>
    #include <iostream>
    #include <lyo/timer.hpp>
    #include <sstream>
#else
    #include <lyo/types.hpp>
#endif

namespace hal
{
    class debug
    {
      public:

        enum severity : lyo::u8
        {
            info,
            warning,
            error
        };

#ifndef NDEBUG
        /* Output any amount of arguments to stdout/stderr and an output file. */
        template <typename... Args>
        static void print(severity type, Args... args) noexcept
        {
            std::stringstream ss;

            ss << std::fixed << std::setprecision(3) << '[' << m_timer() << "s]\t";

            switch (type)
            {
                case severity::info:
                    ss << "[info]\t";
                    break;

                case severity::warning:
                    ss << "[warning]\t";
                    break;

                case severity::error:
                    ss << "[error]\t";
                    break;
            }

            /* Fold expression to properly output every argument. */
            (ss << ... << args);

            const std::string str { ss.str() };

            m_output << str << std::endl;
            (type == severity::error ? std::cerr : std::cout) << str << std::endl;
        }

        /* Show a message box with an error message. */
        static void panic(const char* title, const char* message) noexcept;

        /* Check a condition, and panic if it's false. */
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

    #define HAL_DEBUG_PRINT(...)                 (static_cast<void>(0))
    #define HAL_DEBUG_PANIC(title, message)      (static_cast<void>(0))
    #define HAL_DEBUG_PRINT(condition, if_false) (static_cast<void>(condition))  // Make sure functions with side effects get called.

#endif