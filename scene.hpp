#pragma once

#include <halcyon/image_loader.hpp>
#include <halcyon/input_handler.hpp>
#include <halcyon/mixer.hpp>
#include <halcyon/ttf_engine.hpp>
#include <halcyon/window.hpp>

#include "level.hpp"

/* scene.hpp:
   An all-in-one package with everything needed for the game. */

namespace hal
{
    class engine;
}

namespace game
{
    class entity;

    class scene
    {
      public:

        scene(hal::engine& eng, lyo::u32 freq);

        // @brief A predefined loop, so that you have less to worry about.
        void main_loop() noexcept;

        // @brief Update the scene and all its components.
        // @returns Whether to quit. Enables neat wrapping inside of a
        // while (scene.update()) {...} block.
        bool update() noexcept;

      private:

        void intro() noexcept;

        hal::input_handler m_input;

        level m_level;

        hal::mixer m_mixer;

        hal::window m_window;

        MAYBE_EMPTY hal::image_loader m_iLoader;
        MAYBE_EMPTY hal::ttf_engine m_ttfEngine;

        const hal::font m_font;

        lyo::precise_timer m_delta;
    };
}  // namespace game