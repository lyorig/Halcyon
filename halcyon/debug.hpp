#pragma once

// debug.hpp:
// Various debugging functions. Also hopefully the only
// part of Halcyon that extensively uses preprocessor defines.

#ifndef NDEBUG
    #define HAL_DEBUG_ENABLED
#endif

// Manual debug switch.
// #define HAL_DEBUG_ENABLED

#ifdef HAL_DEBUG_ENABLED

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

    #include <SDL2/SDL_messagebox.h>

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
            info    = color::white,
            warning = color::orange,
            error   = color::red,
            init    = color::green,
            load    = color::cyan
        };

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        template <typename... Args>
        static void print(Args&&... args)
        {
            debug::print_severity(info, std::forward<Args>(args)...);
        }

        // Output any amount of arguments to stdout/stderr, the console and an output file.
        // This overload additionally specifies the type of message to output.
        template <typename... Args>
        static void print(severity sev, Args&&... args)
        {
            debug::print_severity(sev, std::forward<Args>(args)...);
        }

        // Show a message box with an error message.
        template <typename... Args>
        static void panic(const char* where, std::string what,
            Args&&... args)
        {
            const std::string constructed { lyo::string_from_pack(std::forward<Args>(args)...) };

            debug::print_severity(error, __func__, ": ", what, " failed in ", where, ": ",
                constructed);

            constexpr SDL_MessageBoxButtonData buttons[] {
                { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Exit" },
                { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Run anyway" }
            };

            const std::string msgbox_info {
                lyo::string_from_pack("Function: ", where, "\nInfo: ", constructed)
            };

            const SDL_MessageBoxData msgbox {
                SDL_MESSAGEBOX_ERROR, nullptr, what.c_str(), msgbox_info.c_str(),
                static_cast<int>(std::size(buttons)), buttons, nullptr
            };

            int response { 0 };

            if (::SDL_ShowMessageBox(&msgbox, &response) < 0) [[unlikely]]
            {
                debug::print_severity(error, __func__,
                    ": Message box creation failed, exiting");
                goto Exit;
            }

            else
                debug::print_severity(info, __func__, ": User chose to ",
                    response == 0 ? "exit" : "continue execution");

            if (response == 0) [[likely]]
            {
            Exit:
                std::exit(EXIT_FAILURE);
            }
        }

        // Check a condition, and panic if it's false.
        template <typename... Args>
        static void verify(bool condition, const char* cond_string, const char* func,
            Args&&... extra_info)
        {
            if (!condition) [[unlikely]]
                debug::panic(func, cond_string, std::forward<Args>(extra_info)...);
        }

        static void draw(renderer& rnd, const font& fnt);

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

    #define HAL_DEBUG(...) __VA_ARGS__
    #define HAL_PRINT      hal::debug::print
    #define HAL_PANIC(...) hal::debug::panic(__PRETTY_FUNCTION__, __VA_ARGS__)

    #define HAL_ASSERT(cond, ...) HAL_ASSERT_VITAL(cond, __VA_ARGS__)
    #define HAL_ASSERT_VITAL(cond, ...) \
        hal::debug::verify(cond, #cond, __PRETTY_FUNCTION__, __VA_ARGS__)

    #define HAL_DRAW_CONSOLE hal::debug::draw

#else

    #define HAL_DEBUG(...)

    #define HAL_PRINT(...)
    #define HAL_PANIC(...)

    #define HAL_ASSERT(...)
    #define HAL_ASSERT_VITAL(condition, ...) (void(condition))

    #define HAL_DRAW_CONSOLE(...)

#endif