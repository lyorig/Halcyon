#pragma once

// debug.hpp:
// Various debugging functions. Also hopefully the only
// part of Halcyon that extensively uses preprocessor defines.

// If the application is in debug mode, so is Halcyon.
#ifndef NDEBUG
    #define HAL_DEBUG_ENABLED
#endif

#ifdef HAL_DEBUG_ENABLED

    // MSVC has its own version, because of course it does.
    #if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
        #define __PRETTY_FUNCTION__ __FUNCSIG__
    #endif

    #include <halcyon/other/printing.hpp>
    #include <lyo/timer.hpp>
    #include <lyo/utility.hpp>

    #include <array>
    #include <fstream>
    #include <iostream>
    #include <sstream>

namespace hal
{
    class renderer;
    class font;

    constexpr bool debug_enabled {
    #ifdef HAL_DEBUG_ENABLED
        true
    #else
        false
    #endif
    };

    class debug
    {
    public:
        enum severity
        {
            info,
            warning,
            error,
            init,
            load
        };

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        template <printable... Args>
        static void print(Args&&... args)
        {
            debug::print_severity(info, std::forward<Args>(args)...);
        }

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        // This overload additionally specifies the type of message to output.
        template <printable... Args>
        static void print(severity sev, Args&&... args)
        {
            debug::print_severity(sev, std::forward<Args>(args)...);
        }

        // Show a message box with an error message.
        template <printable... Args>
        static void panic(const char* what, const char* where, Args&&... args)
        {
            debug::print_severity(error, what, " in ", where, ": ", lyo::string_from_pack(std::forward<Args>(args)...));

            std::exit(EXIT_FAILURE);
        }

        template <printable... Args>
        static void warn_if(bool condition, Args&&... extra_info)
        {
            if (condition) [[unlikely]]
                debug::print(warning, std::forward<Args>(extra_info)...);
        }

        // Check a condition, and panic if it's false.
        template <printable... Args>
        static void verify(bool condition, const char* cond_string, const char* func,
            Args&&... extra_info)
        {
            if (!condition) [[unlikely]]
                debug::panic(cond_string, func, std::forward<Args>(extra_info)...);
        }

        static void draw(renderer& rnd, const font& fnt);

    private:
        template <printable... Args>
        static void print_severity(severity type, Args&&... args)
        {
            std::stringstream fwd_info, message;

            fwd_info << std::fixed << std::setprecision(3) << '[' << m_timer()
                     << "s] ";

            switch (type)
            {
            case info:
                fwd_info << "[info]  ";
                break;

            case warning:
                fwd_info << "[warn]  ";
                break;

            case error:
                fwd_info << "[error] ";
                break;

            case init:
                fwd_info << "[init]  ";
                break;

            case load:
                fwd_info << "[load]  ";
                break;

            default:
                fwd_info << "[????]  ";
                break;
            }

            const std::string msg { lyo::string_from_pack(args...) };

            const std::string with_info { fwd_info.str() + msg };

            m_output << with_info << std::endl;
            (type == error ? std::cerr : std::cout) << with_info << std::endl;
        }

        static std::ofstream            m_output;
        static const lyo::precise_timer m_timer;
    };
} // namespace hal

    #define HAL_DEBUG(...) __VA_ARGS__
    #define HAL_PRINT      hal::debug::print
    #define HAL_PANIC(...) hal::debug::panic(__PRETTY_FUNCTION__, __VA_ARGS__)

    #define HAL_WARN_IF(cond, ...) hal::debug::warn_if(cond, __VA_ARGS__)

    #define HAL_ASSERT(cond, ...) HAL_ASSERT_VITAL(cond, __VA_ARGS__)
    #define HAL_ASSERT_VITAL(cond, ...) \
        hal::debug::verify(cond, #cond, __PRETTY_FUNCTION__, __VA_ARGS__)

#else

    #define HAL_DEBUG(...)

    #define HAL_PRINT(...)
    #define HAL_PANIC(...)

    #define HAL_WARN_IF(...)

    #define HAL_ASSERT(...)
    #define HAL_ASSERT_VITAL(condition, ...) (void(condition))

#endif