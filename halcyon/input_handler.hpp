#pragma once

#include <SDL2/SDL_scancode.h>

#include <lyo/bitset.hpp>

#include "internal/subsystem.hpp"

namespace halcyon
{
    class engine;

    class input_handler
    {
      public:

        enum mouse_buttons
        {
            lmb = SDL_NUM_SCANCODES,
            mmb,
            rmb,
            x1,
            x2
        };

        input_handler(engine& engine) noexcept;

        void update() noexcept;

        bool pressed(SDL_Scancode key) const noexcept;
        bool held(SDL_Scancode key) const noexcept;
        bool released(SDL_Scancode key) const noexcept;

      private:

        /* The usual scancodes, plus mouse buttons. */
        lyo::bitset<SDL_NUM_SCANCODES + 5, lyo::u64> m_pressed, m_held, m_released;

        engine& m_engine;
    };
}  // namespace halcyon