#pragma once

#include <SDL2/SDL_events.h>

#include <halcyon/debug.hpp>
#include <halcyon/enums/buttons.hpp>
#include <halcyon/types/packed_array.hpp>
#include <halcyon/types/render.hpp>
#include <lyo/bitset.hpp>
#include <utility>

namespace hal
{
    class engine;

    template <typename Derived>
    class input_base
    {
    public:
        using key_storage = lyo::bitset<SDL_NUM_SCANCODES + (SDL_BUTTON_X2 - SDL_BUTTON_LEFT) + 1>;

        input_base(engine& eng)
        {
            HAL_DEBUG_PRINT(debug::init, "Initialized input handler");
        }

        // Automatic polling. If you want finer control (or use
        // functions not yet implemented), use the poll() function.
        bool update()
        {
            SDL_Event evt;

            while (this->poll(evt))
                if (!static_cast<Derived*>(this)->process(evt))
                    m_ok = false;

            return m_ok;
        }

        void quit()
        {
            m_ok = false;
        }

        // This function doesn't require any storage, so it's
        // included as part of the base.
        bool held(hal::button btn) const
        {
            if (std::to_underlying(btn) <= SDL_NUM_SCANCODES) // Key.
                return ::SDL_GetKeyboardState(nullptr)[static_cast<std::size_t>(btn)];

            else // Mouse button.
                return SDL_BUTTON(std::to_underlying(btn) - SDL_NUM_SCANCODES) & ::SDL_GetMouseState(nullptr, nullptr);
        }

        pixel_pos mouse() const
        {
            point<int> pos;

            ::SDL_GetMouseState(&pos.x, &pos.y);

            return pixel_pos(pos);
        }

    protected:
        bool poll(SDL_Event& event) const
        {
            return ::SDL_PollEvent(&event);
        }

        bool m_ok { true };
    };

    // A basic input handler. Suitable for a single-window application.
    class input_handler : public input_base<input_handler>
    {
    public:
        using input_base::input_base;

        bool update();

        bool pressed(hal::button btn) const;
        bool released(hal::button btn) const;

        bool process(const SDL_Event& event);

    protected:
        key_storage m_pressed, m_released;
    };

    // A basic input handler which queues its
    class queued_input_handler : public input_base<queued_input_handler>
    {
        using holder = packed_array<hal::button, 10, hal::button::none>;

    public:
        using input_base::input_base;

        bool update();

        const holder& pressed() const;
        const holder& held() const;
        const holder& released() const;

        bool process(const SDL_Event& event);

    protected:
        holder m_pressed, m_held, m_released;
    };
} // namespace hal