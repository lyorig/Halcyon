#pragma once

#include "enums/severity.hpp"

#ifndef NDEBUG

    #include <deque>
    #include <lyo/utility.hpp>

    #include "internal/config.hpp"

namespace hal
{
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
                m_queue.pop_front();

            m_queue.emplace_back(lyo::string_from_pack(args...), type);
        }

        static void draw(const font& fnt, const window& wnd) noexcept;

      private:

        using queue_type = std::deque<std::pair<std::string, severity>>;

        static queue_type m_queue;
    };
}  // namespace hal

    #define HAL_CONSOLE_LOG  hal::console::log
    #define HAL_CONSOLE_DRAW hal::console::draw

#else

    #define HAL_CONSOLE_LOG(...)
    #define HAL_CONSOLE_DRAW(...)

#endif