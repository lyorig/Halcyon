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

namespace halcyon
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
        static void panic(const char* reason) noexcept;

        /* Check a condition, and panic if it's false. */
        static void verify(bool condition, const char* if_false) noexcept;

      private:

        static std::ofstream            m_output;
        static const lyo::precise_timer m_timer;
#endif
    };
}  // namespace halcyon

#ifndef NDEBUG

    #define HALCYON_PRINT  halcyon::debug::print
    #define HALCYON_PANIC  halcyon::debug::panic
    #define HALCYON_VERIFY halcyon::debug::verify

#else

    #define HALCYON_PRINT(...)                  (void(0))
    #define HALCYON_PANIC(reason)               (void(0))
    #define HALCYON_VERIFY(condition, if_false) (void(condition))  // Make sure functions with side effects get called.

#endif