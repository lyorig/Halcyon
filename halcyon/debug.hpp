#pragma once

/* debug.hpp:
   Various debugging functions. Also hopefully the only
   part of Halcyon that uses preprocessor #defines. */

#ifndef NDEBUG
    #define HALDEBUG
#endif

// Manual debug switch.
// #define HALDEBUG

#ifdef HALDEBUG

    // MSVC has its own version, because of course it does.
    #if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
        #define __PRETTY_FUNCTION__ __FUNCSIG__
    #endif

    #include <halcyon/internal/printing.hpp>
    #include <halcyon/types/color.hpp>
    #include <lyo/timer.hpp>
    #include <lyo/utility.hpp>

    #include <array>
    #include <fstream>
    #include <iostream>
    #include <sstream>

namespace hal
{
    class window;
    class font;

    class debug
    {
    public:
        enum severity
        {
            info = color::white,
            warning = color::orange,
            error = color::red,
            init = color::green,
            load = color::cyan
        };

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        template <typename... Args>
        static void print(Args&&... args)
        {
            debug::print_severity(info, std::forward<Args>(args)...);
        }

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        // This overload additionally specifies the type of message to output..
        template <typename... Args>
        static void print(severity sev, Args&&... args)
        {
            debug::print_severity(sev, std::forward<Args>(args)...);
        }

        // Show a message box with an error message.
        static void panic(const char* why, const char* where,
            const char* message = nullptr);

        // Check a condition, and panic if it's false.
        static void verify(bool condition, const char* cond_string, const char* func,
            const char* extra_info);

        static void draw(window& wnd, const font& fnt);

    private:
        template <typename... Args>
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

            debug::log(type, msg);

            const std::string with_info { fwd_info.str() + msg };

            m_output << with_info << std::endl;
            (type == error ? std::cerr : std::cout) << with_info << std::endl;
        }

        static void log(severity type, const std::string& msg);

        static std::ofstream            m_output;
        static const lyo::precise_timer m_timer;

        using count_type = lyo::u8;

        using value_pair = std::pair<std::string, severity>;
        using queue_type = std::array<value_pair, 10>; // The size acts as the maximum amount of entries.

        static queue_type m_queue;
        static count_type m_entries;

        static bool m_repaint; // Whether to recreate the texture.
    };
} // namespace hal

    #define HAL_DEBUG(...)       __VA_ARGS__
    #define HAL_DEBUG_PRINT      hal::debug::print
    #define HAL_DEBUG_PANIC(why) hal::debug::panic(why, __PRETTY_FUNCTION__)

    #define HAL_DEBUG_ASSERT(cond, if_false) HAL_DEBUG_ASSERT_VITAL(cond, if_false)
    #define HAL_DEBUG_ASSERT_VITAL(cond, if_false) \
        hal::debug::verify(cond, #cond " failed", __PRETTY_FUNCTION__, if_false)

    #define HAL_DEBUG_DRAW hal::debug::draw

#else

    #define HAL_DEBUG(...)

    #define HAL_DEBUG_PRINT(...)
    #define HAL_DEBUG_PANIC(...)

    #define HAL_DEBUG_ASSERT(...)
    #define HAL_DEBUG_ASSERT_VITAL(condition, ...) (void(condition))

    #define HAL_DEBUG_DRAW(...)

#endif