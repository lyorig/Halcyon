#pragma once

#include "enums/severity.hpp"
#include <algorithm>
#include <array>
#include <halcyon/types/render.hpp>
#include <lyo/utility.hpp>

#ifndef NDEBUG
#define HALDEBUG
#endif

// Manual switch to enable debugging in any mode.
// #define HALDEBUG

#ifdef HALDEBUG
namespace hal
{
    class font;
    class window;

    class console
    {
    public:
        // Log a variadic amount of arguments.
        template <typename... Args>
        static void log(severity type, Args... args)
        {
            if (m_entries == m_queue.size())
            {
                std::rotate(m_queue.begin(), m_queue.begin() + 1, m_queue.end());
                m_queue.back() = { lyo::string_from_pack(args...), type };
            }

            else
                m_queue[m_entries++] = { lyo::string_from_pack(args...), type };

            m_repaint = true;
        }

        // Draw the console to the top left corner of the screen.
        static void draw(const font& fnt, const window& wnd);

    private:
        using count_type = lyo::u8;

        using value_pair = std::pair<std::string, severity>;
        using queue_type = std::array<value_pair, 10>; // The size acts as the maximum amount of entries.

        static queue_type m_queue;
        static count_type m_entries;

        static bool m_repaint; // Whether to recreate the texture.
    };
} // namespace hal

#define HAL_CONSOLE_LOG  hal::console::log
#define HAL_CONSOLE_DRAW hal::console::draw

#else

#define HAL_CONSOLE_LOG(...)
#define HAL_CONSOLE_DRAW(...)

#endif