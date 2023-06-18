#pragma once

#include <list>

#include "components/surface.hpp"
#include "internal/config.hpp"

namespace hal
{
    class font;
    class window;

    class console
    {
      public:

        enum severity : color_t
        {
            info    = white,
            warning = orange,
            error   = red
        };
#ifndef NDEBUG
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

        using queue_type = std::list<std::pair<std::string, severity>>;

        static queue_type m_queue;
#endif
    };
}  // namespace hal

#ifndef NDEBUG

    #define CONSOLE_LOG  hal::console::log
    #define CONSOLE_DRAW hal::console::draw

#else

    #define CONSOLE_LOG(...)           (static_cast<void>(0))
    #define CONSOLE_DRAW(font, window) (static_cast<void>(0))

#endif