#pragma once

#include "enums/severity.hpp"

#ifndef NDEBUG

#include <algorithm>
#include <array>
#include <halcyon/types/render.hpp>
#include <lyo/utility.hpp>

#include "internal/config.hpp"

namespace hal {
class font;
class window;

class console {
public:
    // Log a variadic amount of arguments.
    template <typename... Args>
    static void log(severity type, Args... args)
    {
        if (m_entries == cfg::max_console_entries) {
            std::rotate(m_queue.begin(), m_queue.begin() + 1, m_queue.end());
            m_queue.back() = { lyo::string_from_pack(args...), type };
        }

        else
            m_queue[m_entries++] = { lyo::string_from_pack(args...), type };

        m_repaint = true;
    }

    // Draw the console to the top left corner of the screen. Using
    // the smallest possible font size for performance is recommended.
    static void draw(const font& fnt, const window& wnd);

private:
    using count_type = std::remove_cv_t<decltype(cfg::max_console_entries)>;

    using value_pair = std::pair<std::string, severity>;
    using queue_type = std::array<value_pair, cfg::max_console_entries>;

    static queue_type m_queue;
    static count_type m_entries;

    static bool m_repaint; // Whether to recreate the texture.
};
} // namespace hal

#define HAL_CONSOLE_LOG hal::console::log
#define HAL_CONSOLE_DRAW hal::console::draw

#else

#define HAL_CONSOLE_LOG(...)
#define HAL_CONSOLE_DRAW(...)

#endif