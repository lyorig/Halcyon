#pragma once

#include <deque>
#include <sstream>

#include "internal/config.hpp"
#include "types/colors.hpp"

namespace hal
{

    enum class severity : color_type
    {
        info    = static_cast<color_type>(color::white),
        success = static_cast<color_type>(color::green),
        warning = static_cast<color_type>(color::orange),
        error   = static_cast<color_type>(color::red),
    };
#ifndef NDEBUG

    class font;
    class window;
    class console
    {
      public:

        // Log a variadic amount of arguments.
        template <typename... Args>
        static void log(severity type, Args... args) noexcept
        {
            if (m_queue.size() == cfg::max_console_entries)
            {
                m_queue.pop_front();
            }

            std::stringstream ss;

            ss << std::fixed;

            (ss << ... << args);

            m_queue.emplace_back(ss.str(), type);
        }

        static void draw(const font& fnt, const window& wnd) noexcept;

      private:

        using queue_type = std::deque<std::pair<std::string, severity>>;

        static queue_type m_queue;
    };
}  // namespace hal

    #define CONSOLE_LOG  hal::console::log
    #define CONSOLE_DRAW hal::console::draw

#else

    #define CONSOLE_LOG(...)  (static_cast<void>(0))
    #define CONSOLE_DRAW(...) (static_cast<void>(0))

#endif