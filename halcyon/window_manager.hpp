#pragma once

#include <vector>
#include <halcyon/window.hpp>
#include <halcyon/input_handler.hpp>

namespace hal
{
    class window_manager : public input_handler
    {
    public:

        using input_handler::input_handler;

        template <typename... Args>
        window::id_type add(Args&&... args) noexcept
        {
            return m_windows.emplace_back(std::forward<Args>(args)...).id();
        }

        // Get a window from an ID. Keeping the reference
        // around is not recommended, as it may get removed
        // at any time.
        window& operator[](window::id_type id) noexcept;

        bool update() noexcept final;

    private:

        std::vector<window> m_windows;

        std::vector<window>::iterator find_by_id(window::id_type id) noexcept;

        bool process(const SDL_Event& event) noexcept final;
    };
}