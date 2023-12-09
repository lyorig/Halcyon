#pragma once

/* debug.hpp:
   Various debugging functions. Also hopefully the only
   part of Halcyon that uses preprocessor #defines. */

#include "console.hpp"

#ifndef NDEBUG

// MSVC has its own version, because of course it does.
#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#include <fstream>
#include <iostream>
#include <lyo/timer.hpp>
#include <lyo/utility.hpp>

namespace hal {
class texture;
class debug {
public:
    // Output any amount of arguments to stdout/stderr, the console and an output
    // file.
    template <typename... Args>
    static void print(severity type, Args&&... args)
    {
        std::stringstream fwd_info, message;

        fwd_info << std::fixed << std::setprecision(3) << '[' << m_timer()
                 << "s]\t";

        switch (type) {
        case severity::info:
            fwd_info << "[info]\t\t";
            break;

        case severity::warning:
            fwd_info << "[warning]\t";
            break;

        case severity::error:
            fwd_info << "[error]\t\t";
            break;

        case severity::init:
            fwd_info << "[init]\t\t";
            break;

        case severity::load:
            fwd_info << "[load]\t\t";
            break;

        default:
            fwd_info << "[ThisTypeIsNotInTheSwitchStatementYouDumbass]\t";
            break;
        }

        const std::string msg { lyo::string_from_pack(args...) };

        console::log(type, msg);

        const std::string with_info { fwd_info.str() + msg };

        m_output << with_info << std::endl;
        (type == severity::error ? std::cerr : std::cout) << with_info << std::endl;
    }

    // Show a message box with an error message.
    static void panic(const char* why, const char* where,
        const char* message = nullptr);

    // Check a condition, and panic if it's false.
    static void verify(bool condition, const char* cond_string, const char* func,
        const char* extra_info);

private:
    static std::ofstream m_output;
    static const lyo::precise_timer m_timer;
};
} // namespace hal

#define HAL_DEBUG_PRINT hal::debug::print
#define HAL_DEBUG_PANIC(why) hal::debug::panic(wby, __PRETTY_FUNCTION__)
#define HAL_DEBUG_ASSERT(cond, if_false) \
    hal::debug::verify(cond, #cond " failed", __PRETTY_FUNCTION__, if_false)
#define HAL_DEBUG_CHECK(cond, if_false) HAL_DEBUG_ASSERT(cond, if_false)

#else

#define HAL_DEBUG_PRINT(...)
#define HAL_DEBUG_PANIC(...)
#define HAL_DEBUG_ASSERT(condition, ...) \
    (void(condition)) // Asserts are vital; preserve the condition.
#define HAL_DEBUG_CHECK(...) // Checks are not vital; remove them entirely.

#endif